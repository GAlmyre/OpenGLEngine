#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:

	glm::vec3 _direction = glm::vec3(1.0);

	DirectionalLight();
	DirectionalLight(glm::vec3 position, float intensity, glm::vec3 color, glm::vec3 direction);
	DirectionalLight(const DirectionalLight& light);
	~DirectionalLight();
};

