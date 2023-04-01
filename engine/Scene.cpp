#include "Scene.h"
#include <iostream>

Scene::Scene()
{
	cameraPosition = cameraOrigin = vec2(0, 0);
	cameraScale = vec2(1, 1);
	cameraRotation = 0;
}

void Scene::Init()
{
	vector<Actor*> actors();
}

void Scene::Process(Engine& engine, float delta)
{
	for (Actor* a : actors) a->Process(*this, *engine.GetInput(), delta);
}

void Scene::Draw(Graphics& graphics)
{
	for (Actor* a : actors) graphics.DrawAnimation(a->GetAnimationName(),
		a->GetPosition().x, a->GetPosition().y, a->GetAt(), a->GetLayer(), a->GetRotation());
		//a.GetScale().x, a.GetScale().y);
}

void Scene::AddActor(Actor& actor)
{
	actors.push_back(&actor);
	cout << actors.back() << endl;
}

vector<Actor*>& Scene::GetActors()
{
	return actors;
}
