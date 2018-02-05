#include "Viewer.h"
#include "glm/ext.hpp"
#include <iostream>

Viewer::Viewer()
{
	_cam = Camera(glm::vec3(0.0f, 10.0f, 3.0f));
	setWindow();
	initGBuffer();

	renderLoop();
}

Viewer::Viewer(int width, int height, Camera & cam)
	:_width(width), _height(height), _cam(cam) 
{
	setWindow();
	initGBuffer();
	renderLoop();
}

Viewer::~Viewer()
{
	glfwTerminate();
}

int Viewer::setWindow()
{
	glfwInit();

	// version and profile for openGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// window creation
	_window = glfwCreateWindow(_width, _height, "OpenGL Engine v0.1", NULL, NULL);
	if (_window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(_window);

	// set the basic mouse position
	glfwGetCursorPos(_window, &_mouseLastX, &_mouseLastY);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// input callbacks
	glfwSetWindowUserPointer(_window, this);
	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(_window, mouse_callback);
	glfwSetScrollCallback(_window, scroll_callback);

	// init GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, _width, _height);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// parse shaders
	_pointLightShader = Shader("shaders/pointLight.vert", "shaders/pointLight.frag");
	_dirLightShader = Shader("shaders/dirLight.vert", "shaders/dirLight.frag");
	_lampShader = Shader("shaders/lamp.vert", "shaders/lamp.frag");
	_gBufferShader = Shader("shaders/gBuffer.vert", "shaders/gBuffer.frag");
	
	return 0;
}

void Viewer::initGBuffer()
{
	// generate and bind the gBuffer
	glGenFramebuffers(1, &_gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);

	// position buffer
	glGenTextures(1, &_gPosition);
	glBindTexture(GL_TEXTURE_2D, _gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _gPosition, 0);

	// normal buffer
	glGenTextures(1, &_gNormal);
	glBindTexture(GL_TEXTURE_2D, _gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, _width, _height, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _gNormal, 0);

	// color + specular buffer
	glGenTextures(1, &_gColorSpec);
	glBindTexture(GL_TEXTURE_2D, _gColorSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _gColorSpec, 0);

	_attachments[0] = GL_COLOR_ATTACHMENT0;
	_attachments[1] = GL_COLOR_ATTACHMENT1;
	_attachments[2] = GL_COLOR_ATTACHMENT2;

	glDrawBuffers(3, _attachments);

	glGenRenderbuffers(1, &_rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, _rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _rboDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Viewer::initShadowMap()
{
	// creates the texture that will hold the depth values from the light's perspective
	glGenFramebuffers(1, &_shadowFBO);

	glGenTextures(1, &_shadowMap);
	glBindTexture(GL_TEXTURE_2D, _shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, _shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Viewer::initLights()
{
	DirectionalLight dirLight(glm::vec3(100.f, 501.f, 100.f), .3f, glm::vec3(1.f), glm::vec3(-1.f, -1.f, -1.f));
	_dirLights.push_back(dirLight);

	PointLight pointLight(glm::vec3(15.f, 15.f, 15.f), 0.f, glm::vec3(0.f, 0.f, 1.f), 1.f, 0.045f, 0.0075f);
	_pointLights.push_back(pointLight);
	pointLight = PointLight(glm::vec3(120.f, 15.f, 15.f), 1.f, glm::vec3(1.f, 0.f, 0.f), 1.f, 0.045f, 0.0075f);
	_pointLights.push_back(pointLight);
	pointLight = PointLight(glm::vec3(240.f, 15.f, 15.f), 1.f, glm::vec3(0.f, 1.f, 0.f), 1.f, 0.045f, 0.0075f);
	_pointLights.push_back(pointLight);
}

void Viewer::geometryPass()
{
	// fill the gBuffer
	glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = glm::perspective(glm::radians(_cam.getZoom()), (float)_width / (float)_height, 0.1f, 2000.0f);
	glm::mat4 view = _cam.getViewMatrix();
	glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(0.2, 0.2, 0.2));;

	_gBufferShader.use();
	_gBufferShader.setMat4("projection", projection);
	_gBufferShader.setMat4("view", view);
	_gBufferShader.setMat4("model", model);
	_model->draw(_gBufferShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Viewer::lightPass()
{
	_dirLightShader.use();
	_dirLightShader.setVec3("viewPos", _cam.getPosition());
	// directional lights
	for (unsigned int i = 0; i < _dirLights.size(); i++)
	{
		_dirLightShader.setVec3("light.direction", _dirLights[i]._direction);
		_dirLightShader.setVec3("light.color", _dirLights[i]._color);
		_dirLightShader.setFloat("light.intensity", _dirLights[i]._intensity);

		_dirLightShader.setFloat("shininess", 64);

		renderQuad();
	}

	// point lights
	_pointLightShader.use();
	_pointLightShader.setVec3("viewPos", _cam.getPosition());
	for (unsigned int i = 0; i < _pointLights.size(); i++)
	{
		_pointLightShader.setVec3("light.position", _pointLights[i]._position);
		_pointLightShader.setVec3("light.color", _pointLights[i]._color);
		_pointLightShader.setFloat("light.intensity", _pointLights[i]._intensity);
		_pointLightShader.setFloat("light.constant", _pointLights[i]._constant);
		_pointLightShader.setFloat("light.linear", _pointLights[i]._linear);
		_pointLightShader.setFloat("light.quadratic", _pointLights[i]._quadratic);


		_pointLightShader.setFloat("shininess", 64);
		renderQuad();
	}
}

void Viewer::renderLoop()
{
	initLights();

	_model = new Model("../Models/sponza/sponza.obj");

	_dirLightShader.use();
	_dirLightShader.setInt("gPosition", 0);
	_dirLightShader.setInt("gNormal", 1);
	_dirLightShader.setInt("gColorSpec", 2);

	_pointLightShader.use();
	_pointLightShader.setInt("gPosition", 0);
	_pointLightShader.setInt("gNormal", 1);
	_pointLightShader.setInt("gColorSpec", 2);

	while (!glfwWindowShouldClose(_window))
	{
		processInput();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		geometryPass();
		/*float near_plane = 1.0f, far_plane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);


		glBindFramebuffer(GL_FRAMEBUFFER, _shadowFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		*/
		// lighting pass setup
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, _gColorSpec);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);
		
		lightPass();

		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);

		// check event calls and swap buffers
		glfwSwapBuffers(_window);

		glfwPollEvents();
	}
}

void Viewer::renderQuad()
{
	if (_quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &_quadVAO);
		glGenBuffers(1, &_quadVBO);
		glBindVertexArray(_quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, _quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(_quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void Viewer::processInput()
{
	if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, true);
	_cam.processCameraInput(_window);
}

void Viewer::framebuffeSizeCallback(int width, int height)
{
	// resize the viewport and set the new height and width values
	glViewport(0, 0, width, height);
	_width = width;
	_height = height;
}

void Viewer::mouseCallback(double xPos, double yPos)
{
	float xOffset = xPos - _mouseLastX;
	float yOffset = _mouseLastY - yPos; // reversed since y-coordinates go from bottom to top

	_mouseLastX = xPos;
	_mouseLastY = yPos;

	_cam.processMouseMovement(xOffset, yOffset);
}

void Viewer::scrollCallback(double yOffset)
{
	_cam.processMouseScroll(yOffset);
}
