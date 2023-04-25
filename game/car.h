#pragma once

#include "../engine/Actor.h"
#include <iostream>

class Car : public Actor
{
public:

	Car();

	void Init(vec2 position, float rotation);
	void Process(Scene scene, Input input, float delta) override;

	int GetCurrentStep();
	virtual void SetCurrentStep(int step);

	vec2 GetTarget();
	virtual void SetTarget(vec2 position);

	vec2 getVelocity();


protected:

	void Accelerate(float delta);
	void Reverse(float delta);
	void Deccelerate(float delta, float baseSpeed = 0);

	void SteerLeft(float delta);
	void SteerRight(float delta);

	void SetMaxSpeedMutliplier(float value = 1);
	void SetMinSpeedMultiplier(float value = 1);
	void SetAccelerationMultiplier(float value = 1);
	void SetReverseAccelerationMultiplier(float value = 1);
	void SetDeccelerationMultiplier(float value = 1);
	void SetSteerSpeedMultiplier(float value = 1);

	void StartCollision(Actor* source) override;


private:

	float acceleration, reverseAcceleration, decceleration, maxSpeed, minSpeed, steerSpeed;
	float currentAcceleration, currentReverseAcceleration, currentDecceleration, currentMaxSpeed, currentMinSpeed,
		currentSteerSpeed;

	float speed;
	float maxSpeedMultiplier, minSpeedMultiplier, accelerationMultiplier, reverseAccelerationMultiplier, 
		deccelerationMultiplier, steerSpeedMultiplier;

	int lap;
	int currentStep;
	vec2 target;
};
