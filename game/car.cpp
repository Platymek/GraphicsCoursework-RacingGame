#include "car.h"

Car::Car(vec2 position, float rotation) : Actor("Car", position, rotation, 2)
{
	SetAnimation("Car/move");
}
