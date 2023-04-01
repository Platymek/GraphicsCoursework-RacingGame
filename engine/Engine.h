#pragma once

#include "Graphics.h";
#include "Input.h";
#include "Scene.h";
class Scene;

#include <vector>
using namespace std;

class Engine
{
public:

	Engine();

	void Init(Graphics& graphics, Scene& scene, Input& input);
	void Process(mat4& projectionMatrix);
	
	void SetScene(Scene& scene);
	Graphics* GetGraphics();
	Input* GetInput();


private:

	Graphics* graphics;
	Scene* currentScene;
	Input* input;
};
