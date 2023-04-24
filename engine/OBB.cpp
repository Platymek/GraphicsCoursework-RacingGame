
// Polygon objects implementation

#include <math.h>
#include "OBB.h"

#include <iostream>
using namespace std;

OBB::OBB()
{
	originalVertices[0] = vertices[0] = glm::vec4(0, 0, 0, 0);
	originalVertices[1] = vertices[1] = glm::vec4(0, 0, 0, 0);
	originalVertices[2] = vertices[2] = glm::vec4(0, 0, 0, 0);
	originalVertices[3] = vertices[3] = glm::vec4(0, 0, 0, 0);
}

OBB::OBB(float width, float height, float angle)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	glm::mat4 angleMatrix = rotate(mat4(1.f), angle, glm::vec3(0, 0, 1.f));

	//cout << "funny: " << angle << endl;

	originalVertices[0] = vertices[0] = glm::vec4(-halfWidth, -halfHeight, 0.0, 1.0) * angleMatrix;
	originalVertices[1] = vertices[1] = glm::vec4(halfWidth, -halfHeight, 0.0, 1.0) * angleMatrix;
	originalVertices[2] = vertices[2] = glm::vec4(halfWidth, halfHeight, 0.0, 1.0) * angleMatrix;
	originalVertices[3] = vertices[3] = glm::vec4(-halfWidth, halfHeight, 0.0, 1.0) * angleMatrix;
}

//This function is assuming a matrix in column major order to transform the points.
//The transform points function is multiplying the original vertex positions by the matrix
void OBB::Transform(glm::mat4 matrix)
{
	for (int i = 0; i < 4; i++) vertices[i] = matrix * originalVertices[i];
}

bool OBB::IsColliding(OBB& p2)
{
	float axis[2], len, proj[4];
	// 2 tests for each edge of a polygon = 4 tests ( 4 axes )

	// Axis 1 Poly 1 //
	axis[0] = vertices[1].x - vertices[0].x;
	axis[1] = vertices[1].y - vertices[0].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	axis[0] /= len; axis[1] /= len;  // unit vector

	// project second poly points on axis
	for (int i = 0; i < 4; i++)
		proj[i] = (p2.vertices[i].x - this->vertices[0].x) * axis[0]
		+ (p2.vertices[i].y - this->vertices[0].y) * axis[1];   // dot product

	if (Test(proj, len)) return false;

	// Axis 2 Poly 1 //
	axis[0] = this->vertices[2].x - this->vertices[1].x;
	axis[1] = this->vertices[2].y - this->vertices[1].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	axis[0] /= len; axis[1] /= len;  // unit vector

	// project second poly points on axis
	for (int i = 0; i < 4; i++)
		proj[i] = (p2.vertices[i].x - this->vertices[1].x) * axis[0]
		+ (p2.vertices[i].y - this->vertices[1].y) * axis[1];   // dot product

	if (Test(proj, len)) return false;

	// Axis 3 Poly 2 //
	axis[0] = p2.vertices[1].x - p2.vertices[0].x;
	axis[1] = p2.vertices[1].y - p2.vertices[0].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	axis[0] /= len; axis[1] /= len;  // unit vector

	// project first poly points on axis
	for (int i = 0; i < 4; i++)
		proj[i] = (this->vertices[i].x - p2.vertices[0].x) * axis[0]
		+ (this->vertices[i].y - p2.vertices[0].y) * axis[1];   // dot product

	if (Test(proj, len)) return false;

	// Axis 4 Poly 2 //
	axis[0] = p2.vertices[2].x - p2.vertices[1].x;
	axis[1] = p2.vertices[2].y - p2.vertices[1].y;
	len = sqrt(axis[0] * axis[0] + axis[1] * axis[1]);
	axis[0] /= len; axis[1] /= len;  // unit vector

	// project first poly points on axis
	for (int i = 0; i < 4; i++)
		proj[i] = (this->vertices[i].x - p2.vertices[1].x) * axis[0]
		+ (this->vertices[i].y - p2.vertices[1].y) * axis[1];   // dot product

	if (Test(proj, len)) return false;

	//no separating axis - must be in collision
	return true;
}

bool OBB::Test(float proj[4], float len)
{
	float min = proj[0], max = proj[0];

	// Find minimum and maximum projections - the other two will lie in between and are of no consequence
	for (int i = 1; i <= 3; ++i)
	{
		if (proj[i] < min) min = proj[i];
		else if (proj[i] > max) max = proj[i];
	}
	//cout << "min: "<< min << " max: " << max << endl;

	// comparison noting that the first point of first poly is at distance 0 and the second point at distance len
	// along the edge of the first poly acting as the axis

	if (min > len || max < 0.0)

		return true;
	else
		return false;
}

vec2 OBB::GetVertex(int number)
{
	return vertices[number];
}
