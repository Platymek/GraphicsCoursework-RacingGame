#include "Player.h"

Player::Player(int playerNumber) : Car(playerNumber)
{
	this->playerNumber = playerNumber;

	string keys[4] { "up", "down", "left", "right" };

	keyNames = map<string, string>();

	for (string key : keys) keyNames.insert({ key, key + to_string(playerNumber) });

	string pName = "Player ";
	pName += to_string(playerNumber);

	SetPlayerName(pName);
}

void Player::Process(Scene scene, Input input, float delta)
{
	Car::Process(scene, input, delta);

	// process steering //
	int turn  = input.IsKeyDown(keyNames["right"]) - input.IsKeyDown(keyNames["left"]);

	if (turn == 1) 

		SteerLeft(delta);

	else if (turn == -1) 

		SteerRight(delta);

	// process acceleration //
	int drive = input.IsKeyDown(keyNames["up"]) - input.IsKeyDown(keyNames["down"]);

	if (drive == 1)

		Accelerate(delta);

	else if (drive == -1) 

		Reverse(delta);
	else 
		Deccelerate(delta);
}
