
#ifndef OBB_H
#define OBB_H


#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"

class OBB
{
public:

	OBB(int width = 0, int height = 0);

	void Transform(glm::mat4 matrix);
	bool IsColliding(OBB& p2);
	bool Test(float proj[4], float len);

private:

	glm::vec4 vertices[4];
	glm::vec4 originalVertices[4];
};

#endif