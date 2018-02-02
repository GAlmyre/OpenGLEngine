#include "PointLight.h"



PointLight::PointLight()
{
}

PointLight::PointLight(glm::vec3 position, float intensity, glm::vec3 color,  float constant, float linear, float quadratic)
:Light(position, intensity, color), _constant(constant), _linear(linear), _quadratic(quadratic)
{
}

PointLight::PointLight(const PointLight & light)
	:Light(light._position, light._intensity, light._color),
	_constant(light._constant), _linear(light._linear), _quadratic(light._quadratic)
{
}


PointLight::~PointLight()
{
}
