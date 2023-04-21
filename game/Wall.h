#pragma once

#include "../engine/Actor.h"

class Wall : public Actor
{
public:

	Wall(vec2 position, float rotation, int height);
};
