#include "WinScreen.h"
#include "MatrixRush.h"

WinScreen::WinScreen() : Scene()
{
}

void WinScreen::Init(string message)
{
	this->message = message;
}

void WinScreen::Process(Engine& engine, float delta)
{
	Scene::Process(engine, delta);

	if (engine.GetInput()->IsKeyReleased("uiStart"))
	{
		MatrixRush* game = dynamic_cast<MatrixRush*>(&engine);

		game->BackToMenu();
	}
}

void WinScreen::Draw(Graphics& graphics)
{
	Scene::Draw(graphics);

	graphics.SetBackgroundColours(29.f / 255.f, 43.f / 255.f, 83.f / 255.f);

	graphics.DrawFont("title", message.c_str(),
		vec2(32, graphics.GetScreenHeight() - 96));

	graphics.DrawFont("roboto", "Press Enter to return to the main menu",
		vec2(32, 32));
}
