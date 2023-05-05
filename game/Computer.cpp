#include "Computer.h"

Computer::Computer(ComplexityType complexity) : Car(0)
{
	this->complexity = complexity;
	
	turned = false;
	SetPlayerName("Computer");
}

void Computer::Process(Scene scene, Input input, float delta)
{
	Car::Process(scene, input, delta * 1.05f);

	switch (complexity)
	{
	case (Simple):

		Accelerate(delta);
		SetRotation(GetAngleTo(GetTarget()));
		break;


	case(Steer):

		Accelerate(delta);

		if (!turned)
		{
			float angleFrom = GetAngleFrom(GetTarget()); 

			if (left)
			{
				SteerRight(delta);
			
				if (angleFrom < 0) turned = true;
			}
			else
			{
				SteerLeft(delta);

				if (angleFrom > 0) turned = true;
			}
		}

		break;
	}
}

void Computer::Init(vec2 position, float rotation)
{
	Car::Init(position, rotation);

	turned = false;
}

void Computer::SetCurrentStep(int step)
{
	Car::SetCurrentStep(step);

	turned = false;
}

void Computer::SetTarget(vec2 position)
{
	Car::SetTarget(position);

	left = GetAngleFrom(GetTarget()) > 0;
}

void Computer::StartCollision(Actor* source)
{
	Car::StartCollision(source);

	SetTarget(GetTarget());
}
