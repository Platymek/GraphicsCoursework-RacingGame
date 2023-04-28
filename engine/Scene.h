#pragma once

#include "Actor.h"
class Actor;

#include "Engine.h"
class Engine;

#include "Graphics.h"

#include "..\glm\glm.hpp"
using namespace glm;

#include <vector>
using namespace std;

class Scene
{
public:

	Scene();

	virtual void Init();
	virtual void Process(Engine& engine, float delta);
	virtual void Draw(Graphics& graphics);

	void AddActor(Actor& actor);
	vector<Actor*>& GetActors();

	void DrawLine(vec2 p1, vec2 p2, GLfloat red, GLfloat green, GLfloat blue);
	void DrawPolygon(vector<vec2> vertices, GLfloat red, GLfloat green, GLfloat blue);


protected:

	vec2 cameraPosition, cameraScale, cameraOrigin;
	float cameraRotation;


private:

	vector<Actor*> actors;

	vector<vec4> line1s;
	vector<vec4> line2s;
	vector<GLfloat> lineRed;
	vector<GLfloat> lineGreen;
	vector<GLfloat> lineBlue;

	vector<vector<vec2>> polygons;
	vector<GLfloat> polygonRed;
	vector<GLfloat> polygonGreen;
	vector<GLfloat> polygonBlue;
};
