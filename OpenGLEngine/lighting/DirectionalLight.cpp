#include "DirectionalLight.h"



DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(glm::vec3 position, float intensity, glm::vec3 color, glm::vec3 direction)
	:Light(position, intensity, color), _direction(direction)
{
}


DirectionalLight::~DirectionalLight()
{
}
