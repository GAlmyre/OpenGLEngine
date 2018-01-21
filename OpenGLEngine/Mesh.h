#include <GLAD/glad.h>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void draw();

private:
	GLuint _VBO;
	GLuint _VAO;
	GLuint _EBO;
};

