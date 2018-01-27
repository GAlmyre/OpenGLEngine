#pragma once
#include "../Shader.h"

class Model
{
public:
	Model(char* path);
	~Model();

	void draw(Shader shader);
};

