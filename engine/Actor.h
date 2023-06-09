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
	Actor(string name, vec2 position, float rotation = 0, int layer = 0, int width = 0, int height = 0, 
		bool draw = true);
	
	virtual void Process(Scene scene, Input input, float delta);

	bool CanDraw();

	string GetName();
	vec2 GetPosition(), GetScale(), GetOrigin();
	float GetRotation(), GetAt();
	virtual string GetAnimationName();
	int GetLayer();

	float GetAngleTo(vec2 position);
	float GetAngleTo(Actor actor);

	float GetAngleFrom(vec2 position);
	float GetAngleFrom(Actor actor);

	bool IsColliding(Actor source);
	void DrawCollision(Scene& scene);
	bool GetHasCollision();

	void ProcessCollision(Actor& source);


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

	void RefreshCollision();

	virtual void StartCollision(Actor* source);
	virtual void EndCollision(Actor* source);


private:

	string name, currentAnimationName, nextAnimationName, state;
	float t, at;
	bool useNextAnimation, draw;

	bool hasCollision;
	OBB collision;
	vector<Actor*> currentCollisions;
	vector<Actor*> nextCollisions;
};
