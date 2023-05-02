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
#include "..\FreeType.h"
using namespace freetype;

#include "..\engine\Input.h"
#include "MatrixRush.h"

#include <iostream>
using namespace std;

glm::mat4 ViewMatrix;  // matrix for the modelling and viewing
glm::mat4 ProjectionMatrix; // matrix for the orthographic projection
int screenWidth = 720, screenHeight = 720;

Input input;
MatrixRush game;

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

	ProjectionMatrix = glm::ortho(0.0, 1.0 * screenWidth, 0.0, 1.0 * screenHeight);
}

void init()
{
	FreeImage_Initialise();

	glClearColor(0.0, 0.0, 0.0, 0.0);

	game.Init(input);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void display()
{
	//clear the colour and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);

	game.Process();

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

