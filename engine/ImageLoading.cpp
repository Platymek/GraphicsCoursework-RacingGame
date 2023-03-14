
#include "ImageLoading.h"

#include "..\FreeImage.h"

#include <iostream>
using namespace std;

bool ImageLoading::loadImage(const char* filename, int* width, int* height)
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib = nullptr;
	//pointer to the image data
	BYTE* bits = nullptr;
	//image width and height
	//unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	*width = FreeImage_GetWidth(dib);
	*height = FreeImage_GetHeight(dib);
	
	//if one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return false;

	
	FREE_IMAGE_COLOR_TYPE colType = FreeImage_GetColorType(dib);

	//bits are stored in BGR order.
	if (colType == FIC_RGB)

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_BGR, GL_UNSIGNED_BYTE, bits);
	
	else if (colType == FIC_RGBALPHA)
	
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);
	else
	{
		cout << "Image is not RGB or RGBA" << endl;
		FreeImage_Unload(dib);
		return false;
	}

	FreeImage_Unload(dib);

	return true;
}