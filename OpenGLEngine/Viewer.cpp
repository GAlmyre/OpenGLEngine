#include "Viewer.h"
#include "glm/ext.hpp"

Viewer::Viewer()
{
	_cam = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	setWindow();

	renderLoop();
}

Viewer::Viewer(int width, int height, Camera & cam)
	:_width(width), _height(height), _cam(cam) 
{
	setWindow();
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
	_pointLightShader = Shader("shaders/simple.vert", "shaders/simple.frag");
	_lampShader = Shader("shaders/lamp.vert", "shaders/lamp.frag");

	return 0;
}

void Viewer::renderLoop()
{
	_model = new Model("../Models/sponza/sponza.obj");

	while (!glfwWindowShouldClose(_window))
	{
		processInput();

		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

		glClearColor(0.06f, 0.05f, 0.08f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model, view, projection;
		view = _cam.getViewMatrix();
		projection = glm::perspective(glm::radians(_cam.getZoom()), (float)_width / (float)_height, 0.1f, 1000.0f);
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down

		_pointLightShader.use();
	
		// mesh draw
		_pointLightShader.setMat4("model", model);
		_pointLightShader.setMat4("view", view);
		_pointLightShader.setMat4("projection", projection);

		_model->draw(_pointLightShader);

		// check event calls and swap buffers
		glfwSwapBuffers(_window);

		glfwPollEvents();
	}
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
