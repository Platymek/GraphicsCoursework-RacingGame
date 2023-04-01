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

void Engine::Process(mat4& projectionMatrix)
{
	currentScene->Process(*this, 0.016f);
	currentScene->Draw(*graphics);

	graphics->Process(projectionMatrix);
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
