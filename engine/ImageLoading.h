#pragma once

#include "..\gl\glew.h"

class ImageLoading
{
public:
	static bool loadImage(const char* filename, int* width, int* height);
};