#pragma once

#include "Graphics.h"
#include "Input.h"
#include "Scene.h"
class Scene;

#include "..\glm\glm.hpp"
using namespace glm;

#include <string>
using namespace std;

class Actor
{
public:

	Actor();
	Actor(string name, vec2 position, float rotation = 0, int layer = 0);
	
	virtual void Process(Scene scene, Input input, float delta);

	vec2 GetPosition(), GetScale(), GetOrigin();
	float GetRotation(), GetAt();
	string GetAnimationName();
	int GetLayer();


protected:

	vec2 position, scale, origin;
	float rotation, animationSpeed;
	int layer;

	float GetT();
	void ResetT();

	string GetState();
	virtual void SetState(string state);

	void SetAnimation(string currentAnimationName, string nextAnimationName);
	void SetAnimation(string animationNameName);


private:

	string name, currentAnimationName, nextAnimationName, state;
	float t, at;
	bool useNextAnimation;
};
