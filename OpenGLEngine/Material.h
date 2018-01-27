#include <glm/glm.hpp>

class Material
{
public:

	glm::vec3 _ambient = glm::vec3(1);
	glm::vec3 _diffuse = glm::vec3(1);
	glm::vec3 _specular = glm::vec3(1);
	float _shininess = 32;

	Material();
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
	~Material();
};

