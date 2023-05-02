#pragma once

#include "../engine/Scene.h"

class WinScreen : public Scene
{
public:

	WinScreen();

	void Init(string message);
	void Process(Engine& engine, float delta) override;
	void Draw(Graphics& graphics) override;


private:

	string message;
};
