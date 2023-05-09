#include "Computer.h"

Computer::Computer(ComplexityType complexity) : Car(0)
{
	this->complexity = complexity;
	
	turned = false;
	SetPlayerName("Computer");
}

void Computer::Process(Scene scene, Input input, float delta)
{

	if (!turned) Car::Process(scene, input, delta * 1.05f);
	else		 Car::Process(scene, input, delta * 1.0f);

	switch (complexity)
	{
	case (Simple):

		Accelerate(delta);
		SetRotation(GetAngleTo(GetTarget()));
		break;


	case(Steer):

		Accelerate(delta);


		// Process Turning//

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

		
		// Process Boosting //

		if (GetBoostMeter() == 1) boost = true;
		else if (GetBoostMeter() < 0.5f) boost = false;

		if (GetSpeed() < 0 || boost) Boost(delta);

		break;
	}
}

void Computer::Init(vec2 position, float rotation)
{
	Car::Init(position, rotation);

	turned = false;
	boost = false;
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
