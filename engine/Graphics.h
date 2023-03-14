#pragma once

#include "Animation.h"

#include <map>
#include <vector>
#include <string>
#include <forward_list>

#include "..\GL\freeglut.h"
#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"

using namespace std;

class Graphics
{
public:

	Graphics(int screenWidth, int screenHeight);

	void Init();
	void Process(mat4& projectionMatrix);
	void AddAnimation(string animationName, const char* folderName, const int numberOfFrames, const float period);
	void DrawAnimation(string animationName, int x, int y, float time = 0, int layer = 0, float rotation = 0,
					float xScale = 1, float yScale = 1, float xOrigin = 0, float yOrigin = 0);

	struct DrawRequest;


private:

	map<string, Animation> animations;
	int screenWidth, screenHeight;

	/// <summary>5 layers</summary>
	forward_list<DrawRequest> drawRequests[5];
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
