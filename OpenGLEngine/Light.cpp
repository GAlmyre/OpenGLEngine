#include "Light.h"

Light::Light()
{
}

Light::Light(glm::vec3 position, float intensity, glm::vec3 color)
	:_position(position), _intensity(intensity), _color(color) {}


Light::~Light()
{
}
