#include "Menu.h"

#include "MatrixRush.h"

void Menu::Process(Engine& engine, float delta)
{
	Scene::Process(engine, delta);

	if (engine.GetInput()->IsKeyReleased("uiStart"))
	{
		MatrixRush* game = dynamic_cast<MatrixRush*>(&engine);

		game->StartGame();
	}
}

void Menu::Draw(Graphics& graphics)
{
	graphics.SetBackgroundColours(29.f / 255.f, 43.f / 255.f, 83.f / 255.f);

	graphics.DrawFont("title", "Matrix Rush", 
		vec2(graphics.GetScreenHeight() / 2 - 240, graphics.GetScreenWidth() / 2));

	graphics.DrawFont("roboto", "CMP-5010B Coursework 1 - Hector Selby Reimundez, 100352591",
		vec2(32, graphics.GetScreenHeight() - 46));

	graphics.DrawFont("roboto", "Press Enter to play the game",
		vec2(32, 32));
}
