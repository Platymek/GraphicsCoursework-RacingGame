#pragma once

#include "Graphics.h";
#include "Scene.h";

#include <vector>
using namespace std;

class Engine
{
public:

	Engine(Graphics& graphics, Scene& scene);

	void Process();
	
	void SetScene(Scene& scene);
	Graphics& GetGraphics();

private:

	Graphics graphics;
	Scene currentScene;
};
