#include "Sprite.h"
#include "ImageLoading.h"
#include "..\glm\gtc\matrix_transform.hpp"
#include "..\gl\glew.h"
#include <iostream>

Sprite::Sprite()
{
	vertexArrayObjectID = vertexBufferObjectID[0] = vertexBufferObjectID[1] 
		= vertexBufferObjectID[2] = 0;

	shader = nullptr;
	width = height = 0;
}

void Sprite::Init(const char* filename)
{
	shader = new Shader;

	if (!shader->load("Basic", "./glslfiles/basicTexture.vert", "./glslfiles/basicTexture.frag"))
		std::cout << "failed to load shader" << std::endl;


	//load png image
	int imageHeight = 0;
	int imageWidth = 0;
	GLubyte* pngImageData = nullptr;

	//create the texture on the GPU
	glGenTextures(1, &textureName);
	glBindTexture(GL_TEXTURE_2D, textureName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  //or use GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	bool success = ImageLoading::loadImage(filename, &width, &height);

	if (!success) 
	{
		std::cout << "Unable to load image file" << std::endl;
		glDeleteTextures(1, &textureName);
		return;
	}
	else std::cout << "Image loaded " << std::endl;

	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	float vertex[18]
	{
		-halfWidth,  halfHeight, 0.0,
		-halfWidth, -halfHeight, 0.0,
		 halfWidth, -halfHeight, 0.0,
		-halfWidth,  halfHeight, 0.0,
		 halfWidth,  halfHeight, 0.0,
		 halfWidth, -halfHeight, 0.0,
	};

	float texture[12]
	{
		0.0f, 1.0,
		0.0f, 0.0,
		1.0f, 0.0,
		0.0f, 1.0,
		1.0f, 1.0,
		1.0f, 0.0,
	};

	float colour[18]{};

	for (int i = 0; i < 18; i++) colour[i] = 1;

	//VAO allocation
	glGenVertexArrays(1, &vertexArrayObjectID);

	// First VAO setup
	glBindVertexArray(vertexArrayObjectID);

	// we need three VBOs - one for the vertices and one for the colours
	glGenBuffers(3, vertexBufferObjectID); 

	//Lets set up the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID[0]);

	//initialises data storage of vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertex, GL_STATIC_DRAW);

	//set the position - linked to the position shader input
	GLint vertexLocation = glGetAttribLocation(shader->handle(), "in_Position");
	glEnableVertexAttribArray(vertexLocation);
	glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Now set up the colours
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID[1]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), colour, GL_STATIC_DRAW);

	//set the colour - linked to the colour shader input.
	GLint colorLocation = glGetAttribLocation(shader->handle(), "in_Color");
	glEnableVertexAttribArray(colorLocation);

	//location in shader, number of componentns,  type, normalised, stride, pointer to first attribute
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Now set up the texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID[2]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), texture, GL_STATIC_DRAW);

	//set the texture coords - linked to the texcoord shader input.
	GLint texLocation = glGetAttribLocation(shader->handle(), "in_TexCoord");
	glEnableVertexAttribArray(texLocation);

	//location in shader, number of componentns,  type, normalised, stride, pointer to first attribute
	glVertexAttribPointer(texLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//good practice to bind to 0.
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Sprite::Render(mat4& projectionMatrix, mat4& modelViewMatrix)
{
	glUseProgram(shader->handle());  // use the shader

	//set the DiffuseMap in GLSL to the texture unit 0.
	glUniform1i(glGetUniformLocation(shader->handle(), "DiffuseMap"), 0);

	glBindTexture(GL_TEXTURE_2D, textureName);

	//set the uniform for the projectionmatrix
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ProjectionMatrix"), 1, GL_FALSE, 
		&projectionMatrix[0][0]);

	//pass the uniform for the ModelView matrix to the shader
	glUniformMatrix4fv(glGetUniformLocation(shader->handle(), "ModelViewMatrix"), 1, GL_FALSE, 
		&modelViewMatrix[0][0]);

	//Draw the object
	glBindVertexArray(vertexArrayObjectID); // select first VAO
	glDrawArrays(GL_TRIANGLES, 0, 6); // draw first object

	glBindVertexArray(0); // unbind the vertex array object
	glUseProgram(0); // turn off the current shader
}
