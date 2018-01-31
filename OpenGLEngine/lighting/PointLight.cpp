#include "PointLight.h"



PointLight::PointLight()
{
}

PointLight::PointLight(glm::vec3 position, float intensity, glm::vec3 color,  float constant, float linear, float quadratic)
:Light(position, intensity, color), _constant(constant), _linear(linear), _quadratic(quadratic)
{
}


PointLight::~PointLight()
{
}
