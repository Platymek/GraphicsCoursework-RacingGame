#include "Actor.h"
#include <iostream>

Actor::Actor()
{
	t = at = 0;
	SetState("idle");

	origin = vec2(0, 0);
	scale = vec2(1, 1);
	currentAnimationName = "";

	animationSpeed = 1;
}

Actor::Actor(string name, vec2 position, float rotation, int layer, int width, int height)
{
	this->name = name;
	this->position = position;
	this->rotation = rotation;
	this->layer = layer;

	this->scale = vec2(1, 1);
	this->currentAnimationName = "";
	this->animationSpeed = 1;
	this->t = this->at = 0;

	collision = OBB(width, height, rotation);

	SetState("idle");
}

void Actor::Process(Scene scene, Input input, float delta)
{
	cout << animationSpeed << endl;

	t += delta;
	at += delta * animationSpeed;

	mat4 collisionTransform = translate(mat4(1.0), vec3(position.x, position.y, 0));
	collisionTransform = rotate(collisionTransform, rotation, glm::vec3(0, 0, 1.f));

	collision.Transform(collisionTransform);
}

void Actor::DrawCollision(Graphics& graphics)
{
	collision.Draw(graphics);
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
