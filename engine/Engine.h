#pragma once

#include "Graphics.h";
#include "Scene.h";
class Scene;

#include <vector>
using namespace std;

class Engine
{
public:

	Engine();

	void Init(Graphics& graphics, Scene& scene);
	void Process(mat4& projectionMatrix);
	
	void SetScene(Scene& scene);
	Graphics* GetGraphics();


private:

	Graphics* graphics;
	Scene* currentScene;
};
