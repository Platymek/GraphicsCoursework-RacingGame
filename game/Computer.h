#pragma once

#include "Car.h"

class Computer : public Car
{
public:

	void Process(Scene scene, Input input, float delta) override;
};
