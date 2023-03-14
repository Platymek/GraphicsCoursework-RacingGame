
// Polygon objects implementation

#include <math.h>
#include "OBB.h"

#include <iostream>
using namespace std;


OBB::OBB()
{
	for(int i=0; i<NUMVERTS; i++)
	{
		vertOriginal[i] = vert[i] = glm::vec4(0.0, 0.0, 0.0, 1.0);
	}
}

//This function is assuming a matrix in column major order to transform the points.
//The transform points function is multiplying the original vertex positions by the matrix
//The z coordinate is ignored.
void OBB::transformPoints(glm::mat4 matrix)
{
	for(int i=0; i<NUMVERTS; i++)
	{
		vert[i] = matrix*vertOriginal[i]; 
		vert[i] = matrix*vertOriginal[i];
	}
}

bool OBB::SAT2D(OBB& p2) 
{
	float axis[2],len,proj[4];
	// 2 tests for each edge of a polygon = 4 tests ( 4 axes )
	
	//// Axis 1 Poly 1
	axis[0]=vert[1].x-vert[0].x;
	axis[1]=vert[1].y-vert[0].y;
	len=sqrt(axis[0]*axis[0]+axis[1]*axis[1]);
	axis[0]/=len; axis[1]/=len;  // unit vector

	//// project second poly points on axis
	for(int i=0; i<NUMVERTS; i++)
		proj[i]=(p2.vert[i].x-this->vert[0].x)*axis[0]+(p2.vert[i].y-this->vert[0].y)*axis[1];   // dot product
	if(SATtest(proj,len)) return false;

	// Axis 2 Poly 1
	axis[0]=this->vert[2].x-this->vert[1].x;
	axis[1]=this->vert[2].y-this->vert[1].y;
	len=sqrt(axis[0]*axis[0]+axis[1]*axis[1]);
	axis[0]/=len; axis[1]/=len;  // unit vector
	// project second poly points on axis
	for(int i=0; i<NUMVERTS; i++)
		proj[i]=(p2.vert[i].x-this->vert[1].x)*axis[0]+(p2.vert[i].y-this->vert[1].y)*axis[1];   // dot product
	if(SATtest(proj,len)) return false;

	// Axis 3 Poly 2
	axis[0]=p2.vert[1].x-p2.vert[0].x;
	axis[1]=p2.vert[1].y-p2.vert[0].y;
	len=sqrt(axis[0]*axis[0]+axis[1]*axis[1]);
	axis[0]/=len; axis[1]/=len;  // unit vector
	// project first poly points on axis
	for(int i=0; i<NUMVERTS; i++)
		proj[i]=(this->vert[i].x-p2.vert[0].x)*axis[0]+(this->vert[i].y-p2.vert[0].y)*axis[1];   // dot product
	if(SATtest(proj,len)) return false;

	// Axis 4 Poly 2
	axis[0]=p2.vert[2].x-p2.vert[1].x;
	axis[1]=p2.vert[2].y-p2.vert[1].y;
	len=sqrt(axis[0]*axis[0]+axis[1]*axis[1]);
	axis[0]/=len; axis[1]/=len;  // unit vector
	// project first poly points on axis
	for(int i=0; i<NUMVERTS; i++)
		proj[i]=(this->vert[i].x-p2.vert[1].x)*axis[0]+(this->vert[i].y-p2.vert[1].y)*axis[1];   // dot product
	if(SATtest(proj,len)) return false;

	//no separating axis - must be in collision
	return true;
}

bool OBB::SATtest(float proj[4], float len)
{
	float min=proj[0], max=proj[0];
	for(int i=1;i<=3;++i){  // Find minimum and maximum projections - the other two will lie in between and are of no consequence
		if(proj[i]<min)min=proj[i];
		else if(proj[i]>max)max=proj[i];
	}
	//cout << "min: "<< min << " max: " << max << endl;
	
	// comparison noting that the first point of first poly is at distance 0 and the second point at distance len
	// along the edge of the first poly acting as the axis
	if(min > len || max < 0.0) return true; // found separating axis
	else return false;
}