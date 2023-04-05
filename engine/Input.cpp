#include "Input.h"
#include <iostream>

Input::Input()
{
	map<int, string> keys = map<int, string>();

	set<int> pressedThisFrame = set<int>();

	set<int> pressed = set<int>();
	set<int> down = set<int>();
	set<int> released = set<int>();

	mouseRightPressed = mouseRightDown = mouseRightReleased 
		= mouseLeftPressed = mouseLeftDown = mouseLeftReleased = false;
}

void Input::Process()
{
	pressed.clear();
	released.clear();

	mouseRightPressed = false;
	mouseRightReleased = false;

	mouseLeftPressed = false;
	mouseRightReleased = false;
}

void Input::ProcessKeyDown(int key, bool special)
{
	int k = key + (special ? 0 : 256);

	if (!down.count(k)) pressed.insert(k);
	
	down.insert(k);
}
void Input::ProcessKeyUp(int key, bool special)
{
	int k = key + (special ? 0 : 256);

	down.erase(k);
	released.insert(k);
}

void Input::AddKey(string name, int key, bool special)
{
	keys.insert({ name, key + (special ? 0 : 256)});

	cout << name << ", " << keys[name] << endl;
}

bool Input::IsKeyPressed(string name)
{
	int key = keys[name];
	return pressed.count(key);
}

bool Input::IsKeyDown(string name)
{
	int key = keys[name];
	return down.count(key);
}

bool Input::IsKeyReleased(string name)
{
	int key = keys[name];
	return released.count(key);
}

void Input::ProcessMouseRightDown()
{
	if (!mouseRightDown) mouseRightPressed = true;
	mouseRightDown = true;
}

void Input::ProcessMouseRightUp()
{
	mouseRightDown = false;
	mouseRightReleased = true;
}

bool Input::IsMouseRightPressed()
{
	return mouseRightPressed;
}

bool Input::IsMouseRightDown()
{
	return mouseRightDown;
}

bool Input::IsMouseRightReleased()
{
	return mouseRightReleased;
}

void Input::ProcessMouseLeftDown()
{
	if (!mouseLeftDown) mouseLeftPressed = true;
	mouseLeftDown = true;
}

void Input::ProcessMouseLeftUp()
{
	mouseLeftDown = false;
	mouseLeftReleased = true;
}

bool Input::IsMouseLeftPressed()
{
	return mouseLeftPressed;
}

bool Input::IsMouseLeftDown()
{
	return mouseLeftDown;
}

bool Input::IsMouseLeftReleased()
{
	return mouseLeftReleased;
}

void Input::SetMousePosition(int x, int y)
{
	mousePosition.x = x;
	mousePosition.y = y;
}

vec2 Input::GetMousePosition()
{
	return mousePosition;
}
