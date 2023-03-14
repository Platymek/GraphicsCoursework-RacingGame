
#ifndef OBB_H
#define OBB_H


#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"

// Polygon objects
const int NUMVERTS = 4;

class OBB
{
public:
	OBB();			//constructor

	glm::vec4 vert[NUMVERTS];			
	glm::vec4 vertOriginal[NUMVERTS];

	//member functions
	void transformPoints(glm::mat4 matrix);
	bool SAT2D(OBB& p2);
	bool SATtest(float proj[4], float len);
};

#endif