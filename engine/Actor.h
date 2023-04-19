#pragma once

#include "OBB.h"
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
	Actor(string name, vec2 position, float rotation = 0, int layer = 0, int width = 0, int height = 0);
	
	virtual void Process(Scene scene, Input input, float delta);

	vec2 GetPosition(), GetScale(), GetOrigin();
	float GetRotation(), GetAt();
	string GetAnimationName();
	int GetLayer();

	string GetName();

	bool IsColliding(Actor source);
	virtual void ProcessCollision(Actor& source);
	void DrawCollision(Graphics& graphics);
	bool GetHasCollision();


protected:

	vec2 position, scale, origin;
	float rotation, animationSpeed;
	int layer;

	float GetT();
	void ResetT();

	string GetState();
	virtual void SetState(string state);

	void SetRotation(float rotation);

	void SetAnimation(string currentAnimationName, string nextAnimationName);
	void SetAnimation(string animationNameName);
	void SetAnimationSpeed(float animationSpeed);


private:

	string name, currentAnimationName, nextAnimationName, state;
	float t, at;
	bool useNextAnimation;

	bool hasCollision;
	OBB collision = OBB(0, 0, 0);
};
