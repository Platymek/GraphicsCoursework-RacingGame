#pragma once

#include "Actor.h"
#include "Graphics.h"
#include "Engine.h"

#include "..\glm\glm.hpp"
using namespace glm;

#include <vector>
using namespace std;

class Scene
{
public:

	Scene();

	virtual void Process(Engine& engine, float delta);
	virtual void Draw(Graphics& graphics);

	void AddActor(Actor& actor);
	vector<Actor&>& GetActors();


private:

	vector<Actor&> actors;

	vec2 cameraPosition, cameraScale, cameraOrigin;
	float cameraRotation;
};
