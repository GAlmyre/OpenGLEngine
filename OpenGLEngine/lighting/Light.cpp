#include "Light.h"
#include <stdio.h>
Light::Light()
{
}

Light::Light(glm::vec3 position, float intensity, glm::vec3 color)
	:_position(position), _intensity(intensity), _color(color) 
{
}

Light::Light(const Light & light)
	:_position(light._position), _color(light._color), _intensity(light._intensity)
{
}


Light::~Light()
{
}
