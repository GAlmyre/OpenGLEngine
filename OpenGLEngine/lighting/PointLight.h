#pragma once
#include "Light.h"
class PointLight :
	public Light
{
public:

	float _constant = 1.0f;
	float _linear = 0.045f;
	float _quadratic = 0.0075f;

	PointLight();
	PointLight(glm::vec3 position, float intensity, glm::vec3 color, float constant, float linear, float quadratic);
	PointLight(const PointLight& light);
	~PointLight();
};

