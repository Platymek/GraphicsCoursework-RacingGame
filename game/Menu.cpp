#include "Menu.h"

Menu::Menu() : Scene()
{
}

void Menu::Process(Engine& engine, float delta)
{

}

void Menu::Draw(Graphics& graphics)
{
	graphics.SetBackgroundColours(29.f / 255.f, 43.f / 255.f, 83.f / 255.f);

	graphics.DrawFont("title", "Matrix Race", 
		vec2(graphics.GetScreenHeight() / 2 - 240, graphics.GetScreenWidth() / 2));
}
