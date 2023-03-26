#include "TestScene.h"

TestScene::TestScene() : Scene()
{
	Car c(vec2(128, 128), 0);
	AddActor(c);
}
