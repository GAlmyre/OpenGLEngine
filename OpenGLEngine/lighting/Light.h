#pragma once
#include <glm/glm.hpp>

class Light
{
public:

	glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 _color = glm::vec3(1.0f, 1.0f, 1.0f);
	float _intensity = 1.0f;

	Light();
	Light(glm::vec3 position, float intensity, glm::vec3 color);
	Light(const Light& light);
	~Light();
};

