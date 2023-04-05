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
	void ProcessMouseRightDown();
	void ProcessMouseRightUp();

	bool IsMouseRightPressed();
	bool IsMouseRightDown();
	bool IsMouseRightReleased();

	void ProcessMouseLeftDown();
	void ProcessMouseLeftUp();

	bool IsMouseLeftPressed();
	bool IsMouseLeftDown();
	bool IsMouseLeftReleased();

	void SetMousePosition(int x, int y);
	vec2 GetMousePosition();


private:

	// key id, name
	map<string, int> keys;

	set<int> pressedThisFrame;

	set<int> pressed;
	set<int> down;
	set<int> released;

	bool mouseRightPressed;
	bool mouseRightDown;
	bool mouseRightReleased;

	bool mouseLeftPressed;
	bool mouseLeftDown;
	bool mouseLeftReleased;

	vec2 mousePosition;
};
