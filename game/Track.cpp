#include "Track.h"
#include "Player.h"

Player p1(1);
Player p2(2);

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

Track::Track() : Scene()
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

	SetState(StateType::Edit);
}

void Track::Process(Engine& engine, float delta)
{
	switch (state)
	{
	case StateType::Edit:

		if (engine.GetInput()->IsKeyReleased("start") && coordinates.size() > 3)

			SetState(StateType::Play);

		else if (engine.GetInput()->IsMouseRightPressed() && coordinates.size() > 0)

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
					Road r = Road(mousex, mousey, 48, 0);
					AddRoad(r, true);
				}
			}
		}
		break;

	case StateType::Play:

		Scene::Process(engine, delta);

		for (Wall& w : walls)
		{
			w.Process(*this, *engine.GetInput(), delta);

			w.Process(*this, *engine.GetInput(), delta);

			for (Actor* a : GetActors())
			{
				if (a->GetHasCollision())
					if (a->IsColliding(w)) a->ProcessCollision(w);
			}
		}

		break;
	}
}

void Track::Draw(Graphics& graphics)
{
	Scene::Draw(graphics);

	for (Wall& w : walls)
	{
		w.DrawCollision(graphics);
	}

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

	for (Road r : roads) AddRoad(r, false);
	Connect();
}

void Track::AddRoad(Road road, bool connect)
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

		if (connect) Connect();
	}

	if (coordinates.size() > 3) drawLeftLine = drawRightLine = true;
}

void Track::RemoveRoad()
{
	coordinates.pop_back();
	widths.pop_back();
	types.pop_back();

	angles.pop_back();
	leftBounds.pop_back();
	rightBounds.pop_back();

	if (coordinates.size() > 3) Connect();
	else drawLeftLine = drawRightLine = false;
}

void Track::Connect()
{
	int lastIndex = coordinates.size() - 1;

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

	connected = true;
}

void Track::SetDrawMiddleLine(bool drawMiddleLine)
{
	this->drawMiddleLine = drawMiddleLine;
}

void Track::SetState(StateType state)
{
	switch (state)
	{
	case StateType::Play:

		float startingAngle = atan(coordinates[1].x - coordinates[0].x, coordinates[1].y - coordinates[0].y);

		float startingLeftAngle = startingAngle - (pi<float>() / 2);
		int thirdWidth = widths[0] / 3;

		// generate player 1
		vec2 c1 = coordinates[0];
		c1 += vec2(thirdWidth * sin(startingLeftAngle), thirdWidth * cos(startingLeftAngle));
		p1.Init(c1, startingAngle);
		AddActor(p1);

		// generate player 2
		vec2 c2 = coordinates[0];
		c2 -= vec2(thirdWidth * sin(startingLeftAngle), thirdWidth * cos(startingLeftAngle));
		p2.Init(c2, startingAngle);
		AddActor(p2);

		for (int i = 0; i < coordinates.size(); i++)
		{
			int o = i == 0 ? coordinates.size() - 1 : (i - 1);


			// generate left walls //

			vec2 linbetween = (leftBounds[i] + leftBounds[o]) / vec2(2, 2);
			float lrotation = atan(leftBounds[i].x - leftBounds[o].x, leftBounds[i].y - leftBounds[o].y);

			int lheight = leftBounds[i].x - leftBounds[o].x;
			int lwidth = leftBounds[i].y - leftBounds[o].y;
			int llength = sqrt(lheight * lheight + lwidth * lwidth);

			Wall lw = Wall(linbetween, lrotation, llength);

			walls.push_back(lw);


			// generate right walls //

			vec2 rinbetween = (rightBounds[i] + rightBounds[o]) / vec2(2, 2);
			float rrotation = atan(rightBounds[i].x - rightBounds[o].x, rightBounds[i].y - rightBounds[o].y);

			int rheight = rightBounds[i].x - rightBounds[o].x;
			int rwidth = rightBounds[i].y - rightBounds[o].y;
			int rlength = sqrt(rheight * rheight + rwidth * rwidth);

			Wall rw = Wall(rinbetween, rrotation, rlength);

			walls.push_back(rw);
		}

		break;
	}

	this->state = state;
}

Track::Wall::Wall(vec2 position, float rotation, int height)
	: Actor("Wall", position, rotation, 0, 1, height, false)
{}
