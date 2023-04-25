#pragma once

#include "Car.h"

class Computer : public Car
{
public:

	enum ComplexityType
	{
		Simple,
		Steer,
		SteerEfficient
	};

	Computer(ComplexityType complexity);
	void Process(Scene scene, Input input, float delta) override;

	void SetCurrentStep(int step) override;
	virtual void SetTarget(vec2 position) override;

private:

	ComplexityType complexity;
	bool left, turned;
};
