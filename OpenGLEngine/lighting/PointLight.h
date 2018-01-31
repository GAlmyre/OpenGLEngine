#pragma once
#include "f:\3D\OpenGLEngine\OpenGLEngine\lighting\Light.h"
class PointLight :
	public Light
{
public:

	float _constant = 1.0;
	float _linear = 0.045;
	float _quadratic = 0.0075;

	PointLight();
	PointLight(glm::vec3 position, float intensity, glm::vec3 color, float constant, float linear, float quadratic);
	~PointLight();
};

