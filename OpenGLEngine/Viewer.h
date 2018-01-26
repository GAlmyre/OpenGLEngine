#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera/Camera.h"
#include "Shader.h"
#include "model/Mesh.h"

// a class to hold the window and camera used to render the world
class Viewer
{
public:
	Viewer();
	Viewer(int width, int height, Camera& cam);
	~Viewer();

	// the main program loop
	void renderLoop();

	GLFWwindow* getWindow() { return _window; }

private:
	Camera _cam;
	GLFWwindow* _window;
	Shader _pointLightShader;
	Shader _lampShader;
	Mesh _cube;
	Mesh _light;

	// window dimensions
	int _width = 800;
	int _height = 600;

	// last mouse position
	double _mouseLastX;
	double _mouseLastY;

	// set the window up with _height and _width
	int setWindow();

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

