#include "Engine.h"

Engine::Engine(Graphics& graphics, Scene& scene)
{
	this->graphics = graphics;
	this->currentScene = scene;
}

void Engine::Process()
{
	currentScene.Process(*this, 0.016f);
	currentScene.Draw(graphics);
}

void Engine::SetScene(Scene& scene)
{
	currentScene = scene;
}

Graphics& Engine::GetGraphics()
{
	return graphics;
}
