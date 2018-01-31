#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera/Camera.h"
#include "Shader.h"
#include "model/Model.h"
#include "Material.h"
#include "lighting/DirectionalLight.h"
#include "lighting/PointLight.h"

// a class to hold the window and camera used to render the world
class Viewer
{
public:
	Viewer();
	Viewer(int width, int height, Camera& cam);
	~Viewer();

	// the main program loop
	void renderLoop();

	void renderQuad();

	GLFWwindow* getWindow() { return _window; }

private:
	Camera _cam;
	GLFWwindow* _window;

	// shaders
	Shader _dirLightShader;
	Shader _pointLightShader;
	Shader _lampShader;
	Shader _gBufferShader;

	Model* _model;

	// lights
	std::vector<DirectionalLight> _dirLights;
	std::vector<PointLight> _pointLights;

	// deferred shading variables
	GLuint _gBuffer;
	GLuint _gPosition, _gNormal, _gColorSpec;
	GLuint _attachments[3];
	GLuint _rboDepth;
	GLuint _quadVAO, _quadVBO;

	// window dimensions
	int _width = 1600;
	int _height = 900;

	// last mouse position
	double _mouseLastX;
	double _mouseLastY;

	// set the window up with _height and _width
	int setWindow();

	// set up the gBuffer for deferred shading
	void initGBuffer();

	// handle keystrokes
	void processInput();

	// resize event function
	void framebuffeSizeCallback(int width, int height);

	// mouse move event function
	void mouseCallback(double xPos, double yPos);

	// mouse scroll event function
	void scrollCallback(double yOffset);

	// wrappers for glfw callbacks
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		Viewer* viewer = (Viewer*)glfwGetWindowUserPointer(window);
		viewer->framebuffeSizeCallback(width, height);
	}

	static void mouse_callback(GLFWwindow* window, double xPos, double yPos)
	{
		Viewer* viewer = (Viewer*)glfwGetWindowUserPointer(window);
		viewer->mouseCallback(xPos, yPos);
	}

	static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
	{
		Viewer* viewer = (Viewer*)glfwGetWindowUserPointer(window);
		viewer->scrollCallback(yOffset);
	}
};

