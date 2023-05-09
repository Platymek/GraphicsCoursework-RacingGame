#include "MatrixRush.h"

#include "..\engine\Scene.h"
#include "..\engine\Graphics.h"

#include "Track.h"
#include "Menu.h"
#include "WinScreen.h"

Track track;
Menu menu;
WinScreen winScreen;

Graphics g(720, 720);

MatrixRush::MatrixRush() : Engine()
{
}

void MatrixRush::Init(Input& input)
{
	// Initialise Graphics //

	g.Init();

	g.AddAnimation("Car/idle", "./textures/Car/idle", 1, 1);
	g.AddAnimation("Car/move", "./textures/Car/move", 2, 0.2f);
	g.AddAnimation("Car/boost", "./textures/Car/boost", 2, 0.2f);

	g.AddAnimation("Car/idle/0", "./textures/Car/idle/0", 1, 1);
	g.AddAnimation("Car/move/0", "./textures/Car/move/0", 2, 0.2f);
	g.AddAnimation("Car/boost/0", "./textures/Car/boost/0", 2, 0.2f);

	g.AddAnimation("Car/idle/1", "./textures/Car/idle/1", 1, 1);
	g.AddAnimation("Car/move/1", "./textures/Car/move/1", 2, 0.2f);
	g.AddAnimation("Car/boost/1", "./textures/Car/boost/1", 2, 0.2f);

	g.AddAnimation("Car/idle/2", "./textures/Car/idle/2", 1, 1);
	g.AddAnimation("Car/move/2", "./textures/Car/move/2", 2, 0.2f);
	g.AddAnimation("Car/boost/2", "./textures/Car/boost/2", 2, 0.2f);

	g.AddAnimation("CountDown", "./textures/CountDown", 4, 1.f);

	g.AddFont("roboto", "./fonts/Roboto/Roboto-Regular.ttf", 14);
	g.AddFont("title", "./fonts/Righteous/Righteous-Regular.ttf", 64);


	// Initialise Input //

	input.AddKey("up1", 'w', false);
	input.AddKey("left1", 'a', false);
	input.AddKey("down1", 's', false);
	input.AddKey("right1", 'd', false);
	input.AddKey("boost1", 32, false);

	input.AddKey("up2", GLUT_KEY_UP, true);
	input.AddKey("left2", GLUT_KEY_LEFT, true);
	input.AddKey("down2", GLUT_KEY_DOWN, true);
	input.AddKey("right2", GLUT_KEY_RIGHT, true);
	input.AddKey("boost2", GLUT_KEY_CTRL_R, true);

	input.AddKey("uiUp", 'w', false);
	input.AddKey("uiLeft", 'a', false);
	input.AddKey("uiDown", 's', false);
	input.AddKey("uiRight", 'd', false);
	input.AddKey("ui0", '0', false);
	input.AddKey("ui1", '1', false);
	input.AddKey("ui2", '2', false);
	input.AddKey("uiEscape", '2', true);
	input.AddKey("uiStart", 13, false);


	// Initialise all else //

	menu.Init();
	Engine::Init(g, menu, input);
}

void MatrixRush::Process()
{
	Engine::Process();
}

void MatrixRush::StartGame()
{
	track.Init();
	SetScene(track);
}

void MatrixRush::Win(string message)
{
	winScreen.Init(message);
	SetScene(winScreen);
}

void MatrixRush::BackToMenu()
{
	menu.Init();
	SetScene(menu);
}
