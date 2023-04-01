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
}

void Input::ProcessKeyDown(int key)
{
	if (!down.count(key)) pressed.insert(key);
	
	down.insert(key);
}

void Input::ProcessKeyDown(unsigned char key)
{
	ProcessKeyDown((int)key + 256);
}

void Input::ProcessKeyUp(int key)
{
	down.erase(key);
	released.insert(key);
}

void Input::ProcessKeyUp(unsigned char key)
{
	ProcessKeyUp((int)key + 256);
}

void Input::AddKey(string name, int key)
{
	keys.insert({ name, key });

	cout << name << ", " << keys[name] << endl;
}

void Input::AddKey(string name, unsigned char key)
{
	AddKey(name, (int)key + 256);
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
