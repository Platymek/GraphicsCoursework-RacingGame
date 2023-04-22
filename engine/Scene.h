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


protected:

	vec2 cameraPosition, cameraScale, cameraOrigin;
	float cameraRotation;


private:

	vector<Actor*> actors;
};
