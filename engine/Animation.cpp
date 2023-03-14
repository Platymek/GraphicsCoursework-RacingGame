#include "Animation.h"
#include <iostream>
#include <string>

using namespace std;

Animation::Animation()
{
	period = 0;
}

void Animation::Init(const char* folderName, const int numberOfFrames, const float period)
{
	this->numberOfFrames = numberOfFrames;
	this->period = period;

	string filename;

	for (int i = 0; i < numberOfFrames; i++)
	{
		sprites.push_back((*new Sprite));

		filename = string(folderName);
		filename += "/" + to_string(i) + ".png";

		sprites[i].Init(filename.c_str());
	}
}

void Animation::RenderFrame(mat4& projectionMatrix, mat4& modelViewMatrix, const int frame)
{
	// render specified frame
	sprites[frame].Render(projectionMatrix, modelViewMatrix);
}

void Animation::Render(mat4& projectionMatrix, mat4& modelViewMatrix, const float time)
{
	// calculate current frame using time and period
	int frame = 0;

	if (time > 0) frame = fmod(time / period, numberOfFrames);

	RenderFrame(projectionMatrix, modelViewMatrix, frame);
}
