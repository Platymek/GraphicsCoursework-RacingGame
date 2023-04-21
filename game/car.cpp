#include "car.h"

float CARTAU = pi<float>() * 2;
float HALFPI = pi<float>() / 2;

Car::Car() : Actor("Car", vec2(0,0), 0, 0, 20, 24)
{
	SetAnimation("move");

	acceleration = 16;
	SetAccelerationMultiplier();

	reverseAcceleration = 16;
	SetReverseAccelerationMultiplier();

	decceleration = 8;
	SetDeccelerationMultiplier();

	maxSpeed = 64;
	SetMaxSpeedMutliplier();

	minSpeed = -32;
	SetMinSpeedMultiplier();

	steerSpeed = 1;
	SetSteerSpeedMultiplier();

	speed = 0;

	SetAnimationSpeed(0);
}

vec2 Car::getVelocity()
{
	vec2 v(speed * -sin(rotation), speed * cos(rotation));
	return v;
}

void Car::Init(vec2 position, float rotation)
{
	this->position = position;
	this->rotation = rotation;
}

void Car::Process(Scene scene, Input input, float delta)
{
	Actor::Process(scene, input, delta);

	if (speed > 0)

		SetAnimationSpeed(currentMaxSpeed / speed);
	else
		SetAnimationSpeed(0);

	position += getVelocity() * delta;
}

void Car::ProcessCollision(Actor& source)
{
	float angle = atan(position.x - source.GetPosition().x, source.GetPosition().y - position.y) 
				- GetRotation();

	while (angle >  pi<float>()) angle -= CARTAU;
	while (angle < -pi<float>()) angle += CARTAU;

	bool behind = angle > HALFPI || angle < -HALFPI;
	bool movingForwards = speed > 0;

	if (behind && !movingForwards || !behind && movingForwards) speed = -speed;
}

void Car::Accelerate(float delta)
{
	if (speed < maxSpeed) speed += delta * currentAcceleration;
	else Deccelerate(delta, maxSpeed);
}

void Car::Reverse(float delta)
{
	if (speed > minSpeed) speed -= delta * currentReverseAcceleration;
	else Deccelerate(delta, minSpeed);
}

void Car::Deccelerate(float delta, float baseSpeed)
{
	if (speed != baseSpeed)
	{
		float previousSpeed = speed;
		float nextSpeed = speed - delta * currentDecceleration * (speed < baseSpeed ? -1 : 1);

		if ((previousSpeed > baseSpeed && nextSpeed < baseSpeed) 
			|| (previousSpeed < baseSpeed && nextSpeed > baseSpeed))

			speed = baseSpeed;
		else
			speed = nextSpeed;
	}
}

void Car::SteerLeft(float delta)
{
	SetRotation(rotation - delta * currentSteerSpeed);
}

void Car::SteerRight(float delta)
{
	SetRotation(rotation + delta * currentSteerSpeed);
}

void Car::SetMaxSpeedMutliplier(float value)
{
	maxSpeedMultiplier = value;
	currentMaxSpeed = maxSpeed * value;
}

void Car::SetMinSpeedMultiplier(float value)
{
	minSpeedMultiplier = value;
	currentMinSpeed = minSpeed * value;
}

void Car::SetAccelerationMultiplier(float value)
{
	accelerationMultiplier = value;
	currentAcceleration = acceleration * value;
}

void Car::SetReverseAccelerationMultiplier(float value)
{
	reverseAccelerationMultiplier = value;
	currentReverseAcceleration = reverseAcceleration * value;
}

void Car::SetDeccelerationMultiplier(float value)
{
	deccelerationMultiplier = value;
	currentDecceleration = decceleration * value;
}

void Car::SetSteerSpeedMultiplier(float value)
{
	steerSpeedMultiplier = value;
	currentSteerSpeed = steerSpeed * value;
}
