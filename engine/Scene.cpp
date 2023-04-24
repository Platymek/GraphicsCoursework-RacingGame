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
		//if (a->GetHasCollision()) a->DrawCollision(graphics);

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

	mat4 c = mat4(1.f);

	c = translate(c, vec3(graphics.GetScreenWidth() / 2, graphics.GetScreenHeight() / 2, 0));
	
	c = scale		(c, vec3(cameraScale.x, cameraScale.y, 1.f));
	c = rotate		(c, -cameraRotation, vec3(0, 0, 1.f));
	c = translate	(c, vec3(-cameraPosition.x, -cameraPosition.y, 0));

	for (int i = 0; i < line1s.size(); i++)
	{
		vec4 dv1 = c * vec4(line1s[i].x, line1s[i].y, 1.f, 1.f);
		vec4 dv2 = c * vec4(line2s[i].x, line2s[i].y, 1.f, 1.f);

		vec2 v1 = vec2(dv1.x, dv1.y);
		vec2 v2 = vec2(dv2.x, dv2.y);

		graphics.DrawLine(v1, v2);
	}

	line1s.clear();
	line2s.clear();
}

void Scene::AddActor(Actor& actor)
{
	actors.push_back(&actor);
}

vector<Actor*>& Scene::GetActors()
{
	return actors;
}

void Scene::DrawLine(vec2 v1, vec2 v2)
{
	line1s.push_back(vec4(v1.x, v1.y, 1.f, 1.f));
	line2s.push_back(vec4(v2.x, v2.y, 1.f, 1.f));
}
