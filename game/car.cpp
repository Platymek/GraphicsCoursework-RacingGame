#include "car.h"

Car::Car(vec2 position, float rotation) : Actor("Car", position, rotation, 2)
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
}

vec2 Car::getVelocity()
{
	vec2 v(speed * -sin(rotation), speed * cos(rotation));
	return v;
}

void Car::Process(Scene scene, Input input, float delta)
{
	Actor::Process(scene, input, delta);

	position += getVelocity() * delta;
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
	rotation -= delta * currentSteerSpeed;
}

void Car::SteerRight(float delta)
{
	rotation += delta * currentSteerSpeed;
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
