#include <iostream>
using namespace std;

//--- OpenGL ---
#include "..\GL\glew.h"
#include "..\GL\wglew.h"
#pragma comment(lib, "glew32.lib")
//--------------

#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"

#include "..\GL\freeglut.h"
#include "..\FreeImage.h"

#include "..\engine\Graphics.h"
#include "..\engine\Animation.h"
#include "..\engine\Engine.h"
#include "..\engine\Scene.h"
#include "..\engine\Input.h"

#include "Track.h"
#include "Player.h"

#include <iostream>
using namespace std;

glm::mat4 ViewMatrix;  // matrix for the modelling and viewing
glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
int screenWidth = 640, screenHeight = 360;

//booleans to handle when the arrow keys are pressed or released.
bool Left = false;
bool Right = false;
bool Up = false;
bool Down = false;

Player player(2, vec2(128, 128), 0);
Track scene;
Graphics graphics(640, 360);
Engine engine;

Input input;

//OPENGL FUNCTION PROTOTYPES
void display(); //used as callback in glut for display.
void reshape(int width, int height); //used as callback for reshape function in glut
void init(); //called in main when the program starts.

float t;


/*************    START OF OPENGL FUNCTIONS   ****************/

void reshape(int width, int height)		// Resize the OpenGL window
{
	screenWidth = width;
	screenHeight = height;

	// set Viewport dimensions
	glViewport(0, 0, width, height);

	ProjectionMatrix = glm::ortho(0.0, 640.0, 0.0, 360.0);
}

void init()
{
	FreeImage_Initialise();

	glClearColor(0.0, 0.0, 0.0, 0.0);

	t = 0;

	graphics.Init();
	graphics.AddAnimation("Car/move", "./textures/Car/move", 6, 0.6f);

	scene.Init();

	scene.SetDrawMiddleLine(true);

	scene.AddActor(player);

	input.AddKey("up1",		GLUT_KEY_UP,	true);
	input.AddKey("left1",	GLUT_KEY_LEFT,	true);
	input.AddKey("down1",	GLUT_KEY_DOWN,	true);
	input.AddKey("right1",	GLUT_KEY_RIGHT, true);

	input.AddKey("up2",		'w', false);
	input.AddKey("left2",   'a', false);
	input.AddKey("down2",	's', false);
	input.AddKey("right2",	'd', false);

	engine.Init(graphics, scene, input);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void display()
{
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);

	t += 0.0069;

	//if (input.IsMouseDown()) cout << input.GetMousePosition().x << ", " << input.GetMousePosition().y << endl;

	engine.Process();

	glDisable(GL_BLEND);

	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void inputCharacter(unsigned char key, int x, int y)
{
	input.ProcessKeyDown(key, false);
}

void inputCharacterUp(unsigned char key, int x, int y)
{
	input.ProcessKeyUp(key, false);
}

void inputSpecial(int key, int x, int y)
{
	input.ProcessKeyDown(key, true);
}

void inputSpecialUp(int key, int x, int y)
{
	input.ProcessKeyUp(key, true);
}

void mouse(int button, int state, int x, int y)
{
	switch (button) 
	{
	case GLUT_LEFT_BUTTON:

		if (state == GLUT_DOWN)

			input.ProcessMouseLeftDown();
		else
			input.ProcessMouseLeftUp();

		break;

	case GLUT_RIGHT_BUTTON:

		if (state == GLUT_DOWN)

			input.ProcessMouseRightDown();
		else
			input.ProcessMouseRightUp();

		break;
	}
}

void passiveMouse(int x, int y)
{
	input.SetMousePosition(x, y);
}

/**************** END OPENGL FUNCTIONS *************************/


// FREEGLUT WINDOW SET UP
int main(int argc, char **argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Graphics Coursework Racing Game");

	glutReshapeFunc(reshape);

	//This initialises glew - it must be called after the window is created.
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		std::cout << " GLEW ERROR" << endl;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	std::cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	//initialise the objects for rendering
	init();

	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutIdleFunc(idle);

	glutKeyboardFunc(inputCharacter);
	glutKeyboardUpFunc(inputCharacterUp);

	glutSpecialFunc(inputSpecial);
	glutSpecialUpFunc(inputSpecialUp);

	glutMouseFunc(mouse);

	glutMotionFunc(passiveMouse);
	glutPassiveMotionFunc(passiveMouse);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}

