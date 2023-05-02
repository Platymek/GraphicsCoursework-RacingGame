#pragma once

#include "..\engine\Engine.h"

class MatrixRush : public Engine
{
public:

	MatrixRush();

	virtual void Init(Input& input);
	virtual void Process();

	void StartGame();
	void Win(string message);
	void BackToMenu();
};
