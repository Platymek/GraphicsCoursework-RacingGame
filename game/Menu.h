#pragma once

#include "../engine/Scene.h"

class Menu : public Scene
{
public:

	void Process(Engine& engine, float delta) override;
	void Draw(Graphics& graphics) override;
};
