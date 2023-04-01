#pragma once

#include "Car.h"

class Player : public Car
{
public:

	Player(int playerNumber, vec2 position, float rotation);
	void Process(Scene scene, Input input, float delta) override;


private:

	int playerNumber;
	
	map<string, string> keyNames;
};
