#include "Actor.h"
#include <iostream>

Actor::Actor()
{
	t = at = 0;
	SetState("idle");

	origin = vec2(0, 0);
	scale = vec2(1, 1);
}

Actor::Actor(string name, vec2 position, float rotation, int layer)
{
	this->name = name;
	this->position = position;
	this->rotation = rotation;
	this->layer = layer;
}

void Actor::Process(Scene scene, Input input, float delta)
{
	t += delta;
	at += delta * animationSpeed;
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
	cout << name + "/" + animationName << endl;

	this->currentAnimationName = name + "/" + animationName;

	useNextAnimation = false;
}
