#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "../Shader.h"

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture {
	GLuint id;
	std::string type;
	std::string path;
};

class Mesh
{
public:

	// Mes data : vertices, indices and textures
	std::vector<Vertex> _vertices;
	std::vector<GLuint> _indices;
	std::vector<Texture> _textures;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

	void init();

	void draw(Shader shader);

private:
	GLuint _VBO;
	GLuint _VAO;
	GLuint _EBO;

	GLuint _texture;
};

