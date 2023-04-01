#pragma once

#include <map>
#include <set>
#include <string>

using namespace std;

class Input
{
public:

	Input();

	void Process();

	void ProcessKeyDown(int key);
	void ProcessKeyDown(unsigned char key);

	void ProcessKeyUp(int key);
	void ProcessKeyUp(unsigned char key);

	void AddKey(string name, int key);
	void AddKey(string name, unsigned char key);

	bool IsKeyPressed(string name);
	bool IsKeyDown(string name);
	bool IsKeyReleased(string name);


private:

	// key id, name
	map<string, int> keys;

	set<int> pressedThisFrame;

	set<int> pressed;
	set<int> down;
	set<int> released;
};
