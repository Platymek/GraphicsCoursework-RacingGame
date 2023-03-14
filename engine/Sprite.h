#pragma once

#include "..\shaders\Shader.h"
#include "..\glm\glm.hpp"
#include "..\gl\glew.h"

using namespace glm;

class Sprite
{
public:

	Sprite();
	void Init(const char* filename);
	void Render( mat4& projectionMatrix, mat4& modelViewMatrix);


private:

	unsigned int vertexArrayObjectID;
	unsigned int vertexBufferObjectID[3];

	GLuint textureName;
	Shader* shader;
	int width, height;
};
