#include "Input.h"
#include <iostream>

Input::Input()
{
	map<int, string> keys = map<int, string>();

	set<int> pressedThisFrame = set<int>();

	set<int> pressed = set<int>();
	set<int> down = set<int>();
	set<int> released = set<int>();
}

void Input::Process()
{
	pressed.clear();
	released.clear();
	mouseReleased = false;
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

void Input::ProcessMouseDown(int x, int y)
{
	mousePressed = mouseDown;
	mouseDown = true;

	mousePosition = vec2(x, y);
}

void Input::ProcessMouseUp()
{
	mouseDown = false;
	mouseReleased = true;
}

bool Input::IsMousePressed()
{
	return mousePressed;
}

bool Input::IsMouseDown()
{
	return mouseDown;
}

bool Input::IsMouseReleased()
{
	return mouseReleased;
}

vec2 Input::GetMousePosition()
{
	return mousePosition;
}
