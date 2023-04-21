#include "Wall.h"

Wall::Wall(vec2 position, float rotation, int height)
	: Actor("Wall", position, rotation, 0, 1, height, false)
{}
