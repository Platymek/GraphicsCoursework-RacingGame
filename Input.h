#pragma once

class Input
{
public:

	Input();
	void Init();

	void Process();
	void AddKey(string name, int key);


private:

	map<int, string> keysName;

	vector<
};
