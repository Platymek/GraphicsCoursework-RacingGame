#include "TestScene.h"

TestScene::TestScene() : Scene()
{
}

void TestScene::Init()
{
	Car c(vec2(128, 128), 0);
	AddActor(c);
}
