#include "Graphics.h"

#include <iostream>
using namespace std;

Graphics::DrawRequest::DrawRequest(string animationName, int xOrigin, int yOrigin, float time)
{
	this->animationName = animationName;
	this->xOrigin = xOrigin;
	this->yOrigin = yOrigin;
	this->time = time;
}

void Graphics::DrawRequest::AddTransformation(Transformation transformation, float value)
{
	transformationType.push_back(transformation);
	transformationValue.push_back(value);
}

mat4 Graphics::DrawRequest::getModelViewMatrix()
{
	mat4 modelViewMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-xOrigin, -yOrigin, 0.0));

	for (int i = 0; i < transformationType.size(); i++)
		switch (transformationType[i])
		{
		case Transformation::xTranslation:
			modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(transformationValue[i], 0, 0));
			break;

		case Transformation::yTranslation:
			modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0, transformationValue[i], 0));
			break;

		case Transformation::xScale:
			modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3(transformationValue[i], 1.f, 1.f));
			break;

		case Transformation::yScale:
			modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3(1.f, transformationValue[i], 1.f));
			break;

		case Transformation::Rotation:
			modelViewMatrix = glm::rotate(modelViewMatrix, transformationValue[i], glm::vec3(0, 0, 1.f));
			break;
		}

	return modelViewMatrix;
}

string Graphics::DrawRequest::getAnimationName()
{
	return animationName;
}

float Graphics::DrawRequest::getTime()
{
	return time;
}

Graphics::Graphics()
{
	this->screenWidth = 0;
	this->screenHeight = 0;
}

Graphics::Graphics(int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;

	projectionMatrix = glm::ortho(0.0, (double)screenWidth, 0.0, (double)screenHeight);
}

void Graphics::Init()
{

}

void Graphics::Process()
{
	for (forward_list<DrawRequest>& layer : drawRequests)
	{
		for (DrawRequest drawRequest : layer)
		{
			animations[drawRequest.getAnimationName()].Render(projectionMatrix, drawRequest.getModelViewMatrix(),
				drawRequest.getTime());
		}
	
		layer.clear();
	}

	for (int i = 0; i < line1s.size(); i++)
	{
		glBegin(GL_LINES);

		glLineWidth(lineWidths[i]);

		glVertex2f(line1s[i].x / screenWidth - 1, line1s[i].y / screenHeight - 1);
		glVertex2f(line2s[i].x / screenWidth - 1, line2s[i].y / screenHeight - 1);

		glEnd();
	}

	for (int i = 0; i < polygons.size(); i++)
	{
		GLfloat colour[3] = { polygonRed[i], polygonGreen[i], polygonBlue[i], };
		vector<vec2> vertices = polygons[i];

		glBegin(GL_POLYGON);

		glColor3fv(colour);

		cout << colour[0] << endl;

		for (int i = 0; i < vertices.size(); i++)
			glVertex2f(vertices[i].x / screenWidth - 1, vertices[i].y / screenHeight - 1);

		glEnd();
	}

	line1s.clear();
	line2s.clear();
	lineWidths.clear();

	polygons.clear();

	polygonRed.clear();
	polygonGreen.clear();
	polygonBlue.clear();
}

void Graphics::AddAnimation(string animationName, const char* folderName, const int numberOfFrames, 
							const float period)
{
	Animation a;
	a.Init(folderName, numberOfFrames, period);

	animations.insert_or_assign(animationName, a);
}

void Graphics::DrawAnimation(string animationName, int x, int y, float time, int layer, float rotation, float xScale, 
							float yScale, float xOrigin, float yOrigin)
{
	DrawRequest dr(animationName, xOrigin, yOrigin, time);

	dr.AddTransformation(DrawRequest::Transformation::xTranslation, x - xOrigin);
	dr.AddTransformation(DrawRequest::Transformation::yTranslation, y - yOrigin);

	dr.AddTransformation(DrawRequest::Transformation::Rotation, rotation);
	
	dr.AddTransformation(DrawRequest::Transformation::xScale, xScale);
	dr.AddTransformation(DrawRequest::Transformation::yScale, yScale);

	drawRequests[layer].push_front(dr);
}

void Graphics::DrawAnimation(Graphics::DrawRequest drawRequest, int layer)
{
	drawRequests[layer].push_front(drawRequest);
}

void Graphics::DrawLine(vec2 v1, vec2 v2, int width)
{
	vec2 v1n = (v1 - vec2(1)) * vec2(2);
	vec2 v2n = (v2 - vec2(1)) * vec2(2);

	line1s.push_back(v1n);
	line2s.push_back(v2n);
	lineWidths.push_back(width);
}

void Graphics::DrawPolygon(vector<vec2> vertices, GLfloat red, GLfloat green, GLfloat blue)
{
	polygons.push_back(vertices);

	polygonRed.push_back(red);
	polygonGreen.push_back(green);
	polygonBlue.push_back(blue);
}

int Graphics::GetScreenWidth()
{
	return screenWidth;
}

int Graphics::GetScreenHeight()
{
	return screenHeight;
}


