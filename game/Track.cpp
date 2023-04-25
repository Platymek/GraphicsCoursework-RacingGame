#include "Track.h"
#include "Player.h"
#include "Computer.h"

Computer p1(Computer::Steer);
Computer p2(Computer::Steer);
Computer p3(Computer::Steer);
Computer p4(Computer::Steer);

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
				if (coordinates.size() > 0) DrawLine(coordinates[coordinates.size() - 1], vec2(mousex, mousey));

				if (engine.GetInput()->IsMouseLeftDown())
				{
					Road r = Road(mousex, mousey, 48, 0);
					AddRoad(r, true);
				}
			}
		}

		cameraPosition = vec2(engine.GetGraphics()->GetScreenWidth() / 2, 
			engine.GetGraphics()->GetScreenHeight() / 2);

		break;

	case StateType::Play:

		Scene::Process(engine, delta);

		for (Actor* a : GetActors())
		{
			Car* c = dynamic_cast<Car*>(a);

			if (c)
			{
				int s = c->GetCurrentStep() + 1;
				if (s >= steps.size()) s = 0;

				if (steps[s].IsColliding(*c))
				{
					c->SetCurrentStep(s);

					int ns = s + 1;
					if (ns >= steps.size()) ns = 0;
					c->SetTarget(steps[ns].GetPosition());
				}
			}
		}

		for (Wall& w : walls)
		{
			for (Actor* a : GetActors())
			{
				if (a->GetHasCollision())
					if (a->IsColliding(w)) a->ProcessCollision(w);
			}
		}

		//cameraPosition = GetActors()[0]->GetPosition();
		//cameraRotation = GetActors()[0]->GetRotation();

		break;
	}
}

void Track::Draw(Graphics& graphics)
{
	Scene::Draw(graphics);

	for (Wall& w : walls) w.DrawCollision(*this);
	//for (Wall& s : steps) s.DrawCollision(*this);

	if (GetActors().size() > 1)
		steps[dynamic_cast<Car*>(GetActors()[1])->GetCurrentStep()].DrawCollision(*this);

	if (coordinates.size() > 0)
	{
		if (drawMiddleLine)
		{
			DrawLine(coordinates[0], coordinates[coordinates.size() - 1]);

			for (int i = 1; i < coordinates.size(); i++) DrawLine(coordinates[i - 1], coordinates[i]);
		}

		if (drawLeftLine)
		{
			DrawLine(leftBounds[leftBounds.size() - 1], leftBounds[0]);

			for (int i = 1; i < leftBounds.size(); i++) DrawLine(leftBounds[i - 1], leftBounds[i]);
		}

		if (drawRightLine)
		{
			DrawLine(rightBounds[rightBounds.size() - 1], rightBounds[0]);

			for (int i = 1; i < rightBounds.size(); i++) DrawLine(rightBounds[i - 1], rightBounds[i]);
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
		p1.Init(c1, -startingAngle);

		// generate player 2
		vec2 c2 = coordinates[0];
		c2 -= vec2(thirdWidth * sin(startingLeftAngle), thirdWidth * cos(startingLeftAngle));
		p2.Init(c2, -startingAngle);

		// generate player 3
		vec2 c3 = coordinates[0];
		c3 += vec2(thirdWidth * sin(startingLeftAngle), thirdWidth * cos(startingLeftAngle));
		c3 -= vec2(32 * sin(startingAngle), 32 * cos(startingAngle));
		p3.Init(c3, -startingAngle);

		// generate player 3
		vec2 c4 = coordinates[0];
		c4 -= vec2(thirdWidth * sin(startingLeftAngle), thirdWidth * cos(startingLeftAngle));
		c4 -= vec2(32 * sin(startingAngle), 32 * cos(startingAngle));
		p4.Init(c4, -startingAngle);

		for (int i = 0; i < coordinates.size(); i++)
		{
			int o = i == 0 ? coordinates.size() - 1 : (i - 1);

			// generate steps //

			vec2 inbetween = (leftBounds[i] + rightBounds[i]) / vec2(2, 2);

			int height = leftBounds[i].x - rightBounds[i].x;
			int width = leftBounds[i].y - rightBounds[i].y;
			int length = sqrt(height * height + width * width);

			Wall step = Wall(inbetween, -angles[i] + pi<float>() / 2, length);

			steps.push_back(step);


			// generate left walls //

			vec2 linbetween = (leftBounds[i] + leftBounds[o]) / vec2(2, 2);
			float lrotation = -atan(leftBounds[i].x - leftBounds[o].x, leftBounds[i].y - leftBounds[o].y);

			int lheight = leftBounds[i].x - leftBounds[o].x;
			int lwidth = leftBounds[i].y - leftBounds[o].y;
			int llength = sqrt(lheight * lheight + lwidth * lwidth);

			Wall lw = Wall(linbetween, lrotation, llength);

			walls.push_back(lw);


			// generate right walls //

			vec2 rinbetween = (rightBounds[i] + rightBounds[o]) / vec2(2, 2);
			float rrotation = -atan(rightBounds[o].x - rightBounds[i].x, rightBounds[o].y - rightBounds[i].y);

			int rheight = rightBounds[i].x - rightBounds[o].x;
			int rwidth = rightBounds[i].y - rightBounds[o].y;
			int rlength = sqrt(rheight * rheight + rwidth * rwidth);

			Wall rw = Wall(rinbetween, rrotation, rlength);

			walls.push_back(rw);
		}

		//cameraScale = vec2(2,2);

		p1.SetTarget(coordinates[1]);
		p2.SetTarget(coordinates[1]);
		p3.SetTarget(coordinates[1]);
		p4.SetTarget(coordinates[1]);

		AddActor(p1);
		AddActor(p2);
		AddActor(p3);
		AddActor(p4);

		break;
	}

	this->state = state;
}

Track::Wall::Wall(vec2 position, float rotation, int height)
	: Actor("Wall", position, rotation, 0, 2, height, false)
{
	SetRotation(GetRotation() + rotation);
	RefreshCollision();
}
