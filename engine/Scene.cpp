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
	for (Actor* a : actors)
	{
		a->Process(*this, *engine.GetInput(), delta);

		for (Actor* other : actors)
		{
			if (a != other && other->GetHasCollision())
				if (a->IsColliding(*other)) a->ProcessCollision(*other);
		}
	}
}

void Scene::Draw(Graphics& graphics)
{


	for (Actor* a : actors)
	{
		if (a->GetHasCollision()) a->DrawCollision(graphics);

		if (a->CanDraw())
		{
			Graphics::DrawRequest dr = Graphics::DrawRequest(a->GetAnimationName(), 0, 0, a->GetAt());

			// Screen
			dr.AddTransformation(Graphics::DrawRequest::Transformation::xTranslation, graphics.GetScreenWidth() / 2);
			dr.AddTransformation(Graphics::DrawRequest::Transformation::yTranslation, graphics.GetScreenHeight() / 2);
			dr.AddTransformation(Graphics::DrawRequest::Transformation::xScale, cameraScale.x);
			dr.AddTransformation(Graphics::DrawRequest::Transformation::yScale, cameraScale.y);

			// Camera Transformations //
			dr.AddTransformation(Graphics::DrawRequest::Transformation::Rotation, -cameraRotation);
			dr.AddTransformation(Graphics::DrawRequest::Transformation::xTranslation, -cameraPosition.x);
			dr.AddTransformation(Graphics::DrawRequest::Transformation::yTranslation, -cameraPosition.y);

			// Actor Transformations //
			dr.AddTransformation(Graphics::DrawRequest::Transformation::xTranslation, a->GetPosition().x);
			dr.AddTransformation(Graphics::DrawRequest::Transformation::yTranslation, a->GetPosition().y);
			dr.AddTransformation(Graphics::DrawRequest::Transformation::Rotation, a->GetRotation());

			graphics.DrawAnimation(dr);
		}
	}
}

void Scene::AddActor(Actor& actor)
{
	actors.push_back(&actor);
}

vector<Actor*>& Scene::GetActors()
{
	return actors;
}
