#include "Track.h"

Track::Road::Road()
{
	x = y = width = 0;
	type = RoadType::Normal;
}

Track::Road::Road(int x, int y, int width, int type)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->type = (RoadType)type;
}

Track::Road::Road(int x, int y, int width, RoadType type)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->type = type;
}

Track::Track()
{
	coordinates = vector<vec2>();
	widths = vector<int>();
	angles = vector<float>();

	leftBounds	= vector<vec2>();
	rightBounds = vector<vec2>();

	types = vector<RoadType>();

	connected = false;
	drawMiddleLine = false;
}

void Track::Draw(Graphics& graphics)
{
	Scene::Draw(graphics);

	//for (vec2 c : coordinates)
	//{
	//	
	//}

	graphics.DrawLine(vec2(0, 0), vec2(.5, .5));
}

void Track::Init(vector<Road> roads)
{
	Scene::Init();

	for (Road r : roads) AddRoad(r);
}

void Track::AddRoad(Road road)
{
	coordinates.push_back(vec2(road.x, road.y));
	widths.push_back(road.width);
	types.push_back(road.type);

	angles.push_back(0);
	leftBounds.push_back(vec2());
	rightBounds.push_back(vec2());

	//if (coordinates.size() > 2)
	//{
	//	int lastIndex = coordinates.size() - 1;
	//	vec2 previousCoordinates = coordinates[lastIndex - 2];
	//
	//	float angle = atan(road.x - previousCoordinates.x,
	//					road.y - previousCoordinates.y);
	//
	//	angles[lastIndex - 1] = angle;
	//
	//	float leftAngle = 
	//	leftBounds[lastIndex - 1] = vec2();
	//}
}

void Track::Connect()
{
	

	connected = true;
}

void Track::SetDrawMiddleLine(bool drawMiddleLine)
{
	this->drawMiddleLine = drawMiddleLine;
}
