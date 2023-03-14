#pragma once

#include "Sprite.h"
#include <vector>

class Animation
{
public:

	Animation();
	void Init(const char* folderName, const int numberOfFrames, const float period);
	void RenderFrame(mat4& projectionMatrix, mat4& modelViewMatrix, const int frame);
	void Render(mat4& projectionMatrix, mat4& modelViewMatrix, const float time);


private:

	std::vector<Sprite> sprites;
	int numberOfFrames;
	float period;
};
