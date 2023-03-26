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
#include "TestScene.h"

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

TestScene scene;
Graphics graphics(640, 360);
Engine engine;

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

	engine.Init(graphics, scene);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void display()
{
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);

	t += 0.016f;

	//graphics.DrawAnimation("Car/move", 48, 48, t, 0, t, 4, 4);
	//graphics.DrawAnimation("Car/move", 64, 64, t, 0, t + 1, 4, 4);
	//graphics.DrawAnimation("Car/move", 48, 64, t, 0, t + 1, 4, 4);

	//engine.GetGraphics()->DrawAnimation("Car/move", 64, 64, t, 0, t + 1, 4, 4);
	//engine.GetGraphics()->Process(ProjectionMatrix);

	engine.Process(ProjectionMatrix);

	glDisable(GL_BLEND);

	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
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
		cout << " GLEW ERROR" << endl;
	}

	//Check the OpenGL version being used
	int OpenGLVersion[2];
	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLVersion[1]);
	cout << OpenGLVersion[0] << " " << OpenGLVersion[1] << endl;

	//initialise the objects for rendering
	init();

	//specify which function will be called to refresh the screen.
	glutDisplayFunc(display);

	glutIdleFunc(idle);

	//starts the main loop. Program loops and calls callback functions as appropriate.
	glutMainLoop();

	return 0;
}

