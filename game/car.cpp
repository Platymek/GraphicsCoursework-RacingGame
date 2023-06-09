#include "car.h"

#include "Track.h"
#include "MatrixRush.h"

float CARTAU = pi<float>() * 2;
float HALFPI = pi<float>() / 2;

Car::Car(int costume) : Actor("Car", vec2(0,0), 0, 0, 16, 20)
{
	SetAnimation("move");

	acceleration = 64;
	SetAccelerationMultiplier();

	reverseAcceleration = 64;
	SetReverseAccelerationMultiplier();

	decceleration = 64;
	SetDeccelerationMultiplier();

	maxSpeed = 128;
	SetMaxSpeedMutliplier();

	minSpeed = -48;
	SetMinSpeedMultiplier();

	steerSpeed = pi<float>() * 0.75f;
	SetSteerSpeedMultiplier();

	SetAnimationSpeed(0);

	SetCurrentStep(0);

	this->costume = costume;
}

vec2 Car::getVelocity()
{
	vec2 v(speed * -sin(rotation), speed * cos(rotation));
	return v;
}

float Car::GetSpeed()
{
	return speed;
}

float Car::GetBoostMeter()
{
	return boostMeter;
}

string Car::GetAnimationName()
{
	string s = "/";
	s += to_string(costume);

	return Actor::GetAnimationName() + s;
}

int Car::GetCostume()
{
	return costume;
}

string Car::GetPlayerName()
{
	return playerName;
}

int Car::GetLap()
{
	return lap;
}

void Car::SetPlayerName(string name)
{
	playerName = name;
}

void Car::Init(vec2 position, float rotation)
{
	this->position = position;
	this->rotation = rotation;

	boosting = false;
	wasBoosting = false;

	lap = 1;
	speed = 0;
	boostMeter = 1;
	currentStep = 0;
}

void Car::Process(Scene scene, Input input, float delta)
{
	Actor::Process(scene, input, delta);


	// Process Movement //

	if (speed != 0)
	{
		SetAnimationSpeed(abs(speed / currentMaxSpeed));
		SetSteerSpeedMultiplier(abs(speed / maxSpeed));
	}
	else
	{
		SetAnimationSpeed(0);
		SetSteerSpeedMultiplier(0);
	}


	// Process Boost //

	if (wasBoosting && (!boosting || boosting <= 0))
	{
		SetAccelerationMultiplier(1);
		SetMaxSpeedMutliplier(1);
		SetAnimation("move");

		boostRestartTimer = 0;
		wasBoosting = false;
	}
	
	if (!boosting && boostRestartTimer >= boostRestartLimit) boostMeter = boostMeter >= 1 ? 1 : boostMeter + delta;

	boostRestartTimer = boostRestartTimer >= boostRestartLimit 
		? boostRestartLimit
		: boostRestartTimer + delta * 0.5f;


	// Process Result //

	position += getVelocity() * delta;
	
	boosting = false;
}

int Car::GetCurrentStep()
{
	return currentStep;
}

void Car::SetCurrentStep(int step)
{
	currentStep = step;

	if (step == 0) lap += 1;
}

vec2 Car::GetTarget()
{
	return target;
}

void Car::SetTarget(vec2 position)
{
	target = position;
}

void Car::StartCollision(Actor* source)
{
	Actor::StartCollision(source);

	float angle = GetAngleFrom(*source);

	while (angle >  pi<float>()) angle -= CARTAU;
	while (angle < -pi<float>()) angle += CARTAU;

	bool behind = (angle > HALFPI || angle < -HALFPI);
	bool movingForwards = speed > 0;
	bool isCar = source->GetName() == "Car";

	if ((behind && !movingForwards) || (!behind && movingForwards) || !isCar) speed = -speed;
	
	if (isCar)
	{
		Car* c = dynamic_cast<Car*>(source);
		c->boostMeter = 1;
	}

	boostMeter = 1;
}

void Car::Accelerate(float delta)
{
	if (speed < currentMaxSpeed) speed += delta * currentAcceleration;
	else Deccelerate(delta, currentMaxSpeed);
}

void Car::Reverse(float delta)
{
	if (speed > currentMinSpeed) speed -= delta * currentReverseAcceleration;
	else Deccelerate(delta, currentMinSpeed);
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

void Car::Boost(float delta)
{
	if (boostMeter > 0)
	{
		boosting = true;

		if (!wasBoosting)
		{
			wasBoosting = true;

			SetAccelerationMultiplier(6);
			SetMaxSpeedMutliplier(1.75f);
			SetAnimation("boost");
		}

		boostMeter -= delta;
	}
	else boostMeter = 0;
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
