#include "Engine.h"
#include <iostream>

using namespace std;

Engine::Engine()
{
	this->graphics = nullptr;
	this->currentScene = nullptr;
}

void Engine::Init(Graphics& graphics, Scene& scene, Input& input)
{
	this->graphics = &graphics;
	this->currentScene = &scene;
	this->input = &input;
}

void Engine::Process()
{
	float delta = clock() - previousTime;
	previousTime = clock();

	currentScene->Process(*this, delta / 1000);
	currentScene->Draw(*graphics);

	graphics->Process();
	input->Process();
}

void Engine::SetScene(Scene& scene)
{
	currentScene = &scene;
}

Graphics* Engine::GetGraphics()
{
	return graphics;
}

Input* Engine::GetInput()
{
	return input;
}
