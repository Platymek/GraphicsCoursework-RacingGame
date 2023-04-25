#include "Actor.h"
#include <iostream>

float TAU = pi<float>() * 2;

Actor::Actor()
{
	t = at = 0;
	SetState("idle");

	origin = vec2(0, 0);
	scale = vec2(1, 1);
	currentAnimationName = "";

	animationSpeed = 1;

	hasCollision = false;
}

Actor::Actor(string name, vec2 position, float rotation, int layer, int width, int height, bool draw)
{
	this->name = name;
	this->position = position;
	this->layer = layer;
	this->draw = draw;

	SetRotation(rotation);

	this->scale = vec2(1, 1);
	this->currentAnimationName = "";
	this->animationSpeed = 1;
	this->t = this->at = 0;

	collision = OBB(width, height, this->rotation);

	hasCollision = width != 0 && height != 0;

	RefreshCollision();

	SetState("idle");
}

void Actor::Process(Scene scene, Input input, float delta)
{
	t += delta;
	at += delta * animationSpeed;

	RefreshCollision();

	vector<Actor*> newCollisions;

	for (Actor* na : nextCollisions)
	{
		bool newCollision = true;

		for (Actor* ca : currentCollisions)
		{
			if (na == ca)
			{
				newCollision = false;
				break;
			}
		}

		if (newCollision) newCollisions.push_back(na);
	}

	for (Actor* na : newCollisions) StartCollision(na);

	currentCollisions = nextCollisions;
	nextCollisions.clear();
}

bool Actor::IsColliding(Actor source)
{
	return collision.IsColliding(source.collision);
}

void Actor::ProcessCollision(Actor& source)
{
	nextCollisions.push_back(&source);
}

void Actor::DrawCollision(Scene& scene)
{
	scene.DrawLine(collision.GetVertex(0), collision.GetVertex(1));
	scene.DrawLine(collision.GetVertex(1), collision.GetVertex(1));
	scene.DrawLine(collision.GetVertex(2), collision.GetVertex(1));
	scene.DrawLine(collision.GetVertex(3), collision.GetVertex(0));
}

bool Actor::GetHasCollision()
{
	return hasCollision;
}

bool Actor::CanDraw()
{
	return draw;
}

vec2 Actor::GetPosition()
{
	return position;
}

vec2 Actor::GetScale()
{
	return scale;
}

vec2 Actor::GetOrigin()
{
	return origin;
}

float Actor::GetRotation()
{
	return rotation;
}

float Actor::GetAt()
{
	return at;
}

string Actor::GetAnimationName()
{
	return currentAnimationName;
}

int Actor::GetLayer()
{
	return layer;
}

float Actor::GetAngleTo(vec2 position)
{
	float angle = atan(this->position.x - position.x, position.y - this->position.y);

	while (angle > pi<float>()) angle -= TAU;
	while (angle < -pi<float>()) angle += TAU;

	return angle;
}

float Actor::GetAngleTo(Actor actor)
{
	return GetAngleTo(actor.GetPosition());
}

float Actor::GetAngleFrom(vec2 position)
{
	float angle = GetAngleTo(position) - GetRotation();

	while (angle > pi<float>()) angle -= TAU;
	while (angle < -pi<float>()) angle += TAU;

	return angle;
}

float Actor::GetAngleFrom(Actor actor)
{
	return GetAngleFrom(actor.GetPosition());
}

string Actor::GetName()
{
	return name;
}

float Actor::GetT()
{
	return t;
}

void Actor::ResetT()
{
	t = 0;
}

string Actor::GetState()
{
	return state;
}

void Actor::SetState(string state)
{
	this->state = state;
	ResetT();
}

void Actor::SetRotation(float rotation)
{
	while (rotation >  pi<float>()) rotation -= TAU;
	while (rotation < -pi<float>()) rotation += TAU;

	this->rotation = rotation;
}

void Actor::SetAnimation(string currentAnimationName, string nextAnimationName)
{
	this->currentAnimationName = currentAnimationName;
	this->nextAnimationName = nextAnimationName;

	useNextAnimation = true;
}

void Actor::SetAnimation(string animationName)
{
	string a = string(name);
	a += "/" + animationName;
	this->currentAnimationName = a;

	useNextAnimation = false;
}

void Actor::SetAnimationSpeed(float animationSpeed)
{
	this->animationSpeed = animationSpeed;
}

void Actor::RefreshCollision()
{
	if (hasCollision)
	{
		mat4 collisionTransform = translate(mat4(1.f), vec3(position.x, position.y, 0));
		collisionTransform = rotate(collisionTransform, rotation, glm::vec3(0, 0, 1.f));

		collision.Transform(collisionTransform);
	}
}

void Actor::StartCollision(Actor* source)
{
	cout << GetName() << " has started colliding with " << source->GetName() << endl;
}

void Actor::EndCollision(Actor* source)
{
	cout << GetName() << " has stopped colliding with " << source->GetName() << endl;
}
