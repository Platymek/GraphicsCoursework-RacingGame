#pragma once

#include "Animation.h"

#include <map>
#include <vector>
#include <string>
#include <forward_list>

#include "..\GL\freeglut.h"
#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"

#include "../FreeType.h"
using namespace freetype;

using namespace std;

class Graphics
{
public:

	struct DrawRequest;

	Graphics();
	Graphics(int screenWidth, int screenHeight);

	void Init();
	void Process();
	void AddFont(string name, const char* directory, unsigned int size);
	void DrawFont(string name, const char* text, vec2 position);
	void AddAnimation(string animationName, const char* folderName, const int numberOfFrames, const float period);

	void DrawAnimation(string animationName, int x, int y, float time = 0, int layer = 0, float rotation = 0,
		float xScale = 1, float yScale = 1, float xOrigin = 0, float yOrigin = 0);
	void DrawAnimation(Graphics::DrawRequest drawRequest, int layer = 0);

	void DrawLine(vec2 v1, vec2 v2, GLfloat red, GLfloat green, GLfloat blue);
	void DrawPolygon(vector<vec2> vertices, GLfloat red, GLfloat green, GLfloat blue);

	int GetScreenWidth();
	int GetScreenHeight();
	void SetBackgroundColours(GLfloat red, GLfloat green, GLfloat blue);


private:

	map<string, Animation> animations;
	map<string, Font> fonts;
	int screenWidth, screenHeight;

	/// <summary>5 layers</summary>
	forward_list<DrawRequest> drawRequests[5];

	vector<vec2> line1s;
	vector<vec2> line2s;
	vector<GLfloat> lineRed;
	vector<GLfloat> lineGreen;
	vector<GLfloat> lineBlue;

	vector<vector<vec2>> polygons;
	vector<GLfloat> polygonRed;
	vector<GLfloat> polygonGreen;
	vector<GLfloat> polygonBlue;

	vector<string> texts;
	vector<string> textNames;
	vector<vec2> textPositions;

	mat4 projectionMatrix;
};

struct Graphics::DrawRequest
{
public:

	enum class Transformation
	{
		xTranslation,
		yTranslation,
		xScale,
		yScale,
		Rotation
	};

	DrawRequest(string animationName, int xOrigin, int yOrigin, float time);

	void   AddTransformation(Transformation transformation, float value);
	mat4   getModelViewMatrix();
	string getAnimationName();
	float  getTime();

private:

	string animationName;
	int xOrigin, yOrigin;
	float time;

	vector<Transformation> transformationType;
	vector<float> transformationValue;
};
