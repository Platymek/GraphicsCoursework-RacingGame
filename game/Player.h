#pragma once

#include "Car.h"

class Player : public Car
{
public:

	Player(int playerNumber);
	void Process(Scene scene, Input input, float delta) override;


private:

	int playerNumber;
	
	map<string, string> keyNames;
};
