#pragma once
#include "Light.h"

class DirectionalLight :
	public Light
{
public:

	glm::vec3 _direction;

	DirectionalLight();
	~DirectionalLight();
};
