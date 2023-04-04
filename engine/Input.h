#pragma once

#include <map>
#include <set>
#include <string>

#include "..\glm\glm.hpp"
#include "..\glm\gtc\matrix_transform.hpp"

using namespace std;
using namespace glm;

class Input
{
public:

	Input();

	void Process();

	// keys //
	void ProcessKeyDown(int key, bool special);
	void ProcessKeyUp(int key, bool special);

	void AddKey(string name, int key, bool special);

	bool IsKeyPressed(string name);
	bool IsKeyDown(string name);
	bool IsKeyReleased(string name);

	// mouse //
	void ProcessMouseDown(int x, int y);
	void ProcessMouseUp();

	bool IsMousePressed();
	bool IsMouseDown();
	bool IsMouseReleased();
	vec2 GetMousePosition();


private:

	// key id, name
	map<string, int> keys;

	set<int> pressedThisFrame;

	set<int> pressed;
	set<int> down;
	set<int> released;

	bool mousePressed;
	bool mouseDown;
	bool mouseReleased;
	vec2 mousePosition;
};
