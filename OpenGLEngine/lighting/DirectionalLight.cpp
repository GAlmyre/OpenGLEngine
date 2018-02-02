#include "DirectionalLight.h"
#include <stdio.h>


DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(glm::vec3 position, float intensity, glm::vec3 color, glm::vec3 direction)
	:Light(position, intensity, color), _direction(direction)
{
}

DirectionalLight::DirectionalLight(const DirectionalLight& light)
	: Light(light._position, light._intensity, light._color), _direction(light._direction)
{
}

DirectionalLight::~DirectionalLight()
{
}
