#include "Camera.h"

Camera::~Camera()
{
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
:_front(glm::vec3(0.0f, 0.0f, -1.0f)), _moveSpeed(SPEED), _mouseSens(SENSITIVTY), _zoom(ZOOM)
{
	_position = position;
	_worldUp = up;
	_yaw = yaw;
	_pitch = pitch;

	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
:_front(glm::vec3(0.0f, 0.0f, -1.0f)), _moveSpeed(SPEED), _mouseSens(SENSITIVTY), _zoom(ZOOM)
{
	_position = glm::vec3(posX, posY, posZ);
	_worldUp = glm::vec3(upX, upY, upZ);
	_yaw = yaw;
	_pitch = pitch;

	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(_position, _position + _front, _up);
}

void Camera::processCameraInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		_position += _moveSpeed * _front;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		_position -= _moveSpeed * _front;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		_position -= glm::normalize(glm::cross(_front, _up)) * _moveSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		_position += glm::normalize(glm::cross(_front, _up)) * _moveSpeed;

	updateCameraVectors();
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constraintPitch)
{
	xOffset *= _mouseSens;
	yOffset *= _mouseSens;

	_yaw += xOffset;
	_pitch += yOffset;

	// avoid flipping screen
	if (constraintPitch)
	{
		if (_pitch > 89.0f)
			_pitch = 89.0f;
		if (_pitch < -89.0f)
			_pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset)
{
	if (_zoom >= 1.0f && _zoom <= 45.0f)
		_zoom -= yOffset;
	if (_zoom <= 1.0f)
		_zoom = 1.0f;
	if (_zoom >= 45.0f)
		_zoom = 45.0f;
}

float Camera::getZoom()
{
	return _zoom;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	front.y = sin(glm::radians(_pitch));
	front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(front);

	_right = glm::normalize(glm::cross(_front, _worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	_up = glm::normalize(glm::cross(_right, _front));
}
