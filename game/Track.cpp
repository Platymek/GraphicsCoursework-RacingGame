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

	drawLeftLine = drawRightLine = false;

	editting = true;
}

void Track::Process(Engine& engine, float delta)
{
	if (editting)
	{
		if (engine.GetInput()->IsMouseRightPressed() && coordinates.size() > 0) 
			
			RemoveRoad();
		else
		{
			int mousex = engine.GetInput()->GetMousePosition().x;
			int mousey = (engine.GetGraphics()->GetScreenHeight() - engine.GetInput()->GetMousePosition().y);

			bool valid = true;

			for (int i = 0; i < coordinates.size(); i++)
			{
				vec2 v = coordinates[i];

				int distanceSquared = pow(mousex - v.x, 2) + pow(mousey - v.y, 2);

				if (distanceSquared <= pow(widths[i], 2) * 2)
				{
					valid = false;
					break;
				}
			}

			if (valid)
			{
				if (coordinates.size() > 0)
					engine.GetGraphics()->DrawLine(coordinates[coordinates.size() - 1], vec2(mousex, mousey));

				if (engine.GetInput()->IsMouseLeftDown())
				{
					Road r = Road(mousex, mousey, 16, 0);
					AddRoad(r);
				}
			}
		}
	}
	else Scene::Process(engine, delta);
}

void Track::Draw(Graphics& graphics)
{
	Scene::Draw(graphics);

	if (coordinates.size() > 0)
	{
		if (drawMiddleLine)
		{
			graphics.DrawLine(coordinates[0], coordinates[coordinates.size() - 1]);

			for (int i = 1; i < coordinates.size(); i++)
				graphics.DrawLine(coordinates[i - 1], coordinates[i]);
		}

		if (drawLeftLine)
		{
			graphics.DrawLine(leftBounds[leftBounds.size() - 1], leftBounds[0]);

			for (int i = 1; i < leftBounds.size(); i++)
				graphics.DrawLine(leftBounds[i - 1], leftBounds[i]);
		}

		if (drawRightLine)
		{
			graphics.DrawLine(rightBounds[rightBounds.size() - 1], rightBounds[0]);

			for (int i = 1; i < rightBounds.size(); i++)
				graphics.DrawLine(rightBounds[i - 1], rightBounds[i]);
		}
	}
}

void Track::Init()
{
	Scene::Init();
}

void Track::Init(vector<Road> roads)
{
	Scene::Init();

	for (Road r : roads) AddRoad(r);
	Connect();

	for (vec2 v : leftBounds) cout << v.x << ", " << v.y << endl;
}

void Track::AddRoad(Road road, bool processFirst)
{
	coordinates.push_back(vec2(road.x, road.y));
	widths.push_back(road.width);
	types.push_back(road.type);

	angles.push_back(0);
	leftBounds.push_back(vec2());
	rightBounds.push_back(vec2());

	if (coordinates.size() > 2)
	{
		// process next road //
		int lastIndex = coordinates.size() - 1;
		vec2 previousCoordinates = coordinates[lastIndex - 2];

		float angle = atan(road.x - previousCoordinates.x,
			road.y - previousCoordinates.y);

		angles[lastIndex - 1] = angle;

		float leftAngle = angle - (pi<float>() / 2);

		leftBounds[lastIndex - 1] = coordinates[lastIndex - 1]
			+ vec2(widths[lastIndex - 1] * sin(leftAngle), widths[lastIndex - 1] * cos(leftAngle));

		rightBounds[lastIndex - 1] = coordinates[lastIndex - 1]
			- vec2(widths[lastIndex - 1] * sin(leftAngle), widths[lastIndex - 1] * cos(leftAngle));

		if (processFirst)
		{
			// process first road //
			angles[0] = atan(coordinates[1].x - coordinates[lastIndex].x,
				coordinates[1].y - coordinates[lastIndex].y);

			float firstLeftAngle = angles[0] - (pi<float>() / 2);

			leftBounds[0] = coordinates[0]
				+ vec2(widths[0] * sin(firstLeftAngle), widths[0] * cos(firstLeftAngle));

			rightBounds[0] = coordinates[0]
				- vec2(widths[0] * sin(firstLeftAngle), widths[0] * cos(firstLeftAngle));

			// process second road //
			angles[lastIndex] = atan(coordinates[0].x - coordinates[lastIndex - 1].x,
				coordinates[0].y - coordinates[lastIndex - 1].y);

			float lastLeftAngle = angles[lastIndex] - (pi<float>() / 2);

			leftBounds[lastIndex] = coordinates[lastIndex]
				+ vec2(widths[lastIndex] * sin(lastLeftAngle), widths[lastIndex] * cos(lastLeftAngle));

			rightBounds[lastIndex] = coordinates[lastIndex]
				- vec2(widths[lastIndex] * sin(lastLeftAngle), widths[lastIndex] * cos(lastLeftAngle));
		}
	}

	if (coordinates.size() > 3)
	{
		drawLeftLine = drawRightLine = true;
	}
}

void Track::RemoveRoad()
{
	coordinates.pop_back();
	widths.pop_back();
	types.pop_back();

	angles.pop_back();
	leftBounds.pop_back();
	rightBounds.pop_back();
}

void Track::Connect()
{
	

	connected = true;
}

void Track::SetDrawMiddleLine(bool drawMiddleLine)
{
	this->drawMiddleLine = drawMiddleLine;
}
