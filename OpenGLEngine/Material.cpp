#include "Material.h"



Material::Material()
{}

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
	:_ambient(ambient), _diffuse(diffuse), _specular(specular), _shininess(shininess) 
{}


Material::~Material()
{}
