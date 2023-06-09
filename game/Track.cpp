#include "Track.h"
#include "Player.h"
#include "Computer.h"
#include "MatrixRush.h"

Player p1(1);
Player p2(2);

Computer cp1(Computer::Steer);
Computer cp2(Computer::Steer);
Computer cp3(Computer::Steer);
Computer cp4(Computer::Steer);

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
}

void Track::Process(Engine& engine, float delta)
{
	switch (state)
	{
	case StateType::Edit:

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

				if (distanceSquared <= pow(widths[i], 2)) //* 2)
				{
					valid = false;
					break;
				}
			}

			if (valid)
			{
				if (coordinates.size() > 0) DrawLine(coordinates[coordinates.size() - 1], vec2(mousex, mousey), 
					editLineColour[0], editLineColour[1], editLineColour[2]);

				if (engine.GetInput()->IsMouseLeftDown())
				{
					Road r = Road(mousex, mousey, nextRoadWidth, 0);
					AddRoad(r, true);
				}
			}
		}

		if (engine.GetInput()->IsKeyPressed("uiLeft") && nextRoadWidth > 32) nextRoadWidth -= 8;
		if (engine.GetInput()->IsKeyPressed("uiRight") && nextRoadWidth < 72) nextRoadWidth += 8;

		if (engine.GetInput()->IsKeyReleased("ui0"))
		{
			numberOfPlayers = 0;
			SetState(StateType::Play);
		}
		else if (engine.GetInput()->IsKeyReleased("ui1"))
		{
			numberOfPlayers = 1;
			SetState(StateType::Play);
		}
		else if (engine.GetInput()->IsKeyReleased("ui2"))
		{
			numberOfPlayers = 2;
			SetState(StateType::Play);
		}

		cameraPosition = vec2(engine.GetGraphics()->GetScreenWidth() / 2, 
			engine.GetGraphics()->GetScreenHeight() / 2);

		break;

	case StateType::Play:

		if (countDown < 0)

			Scene::Process(engine, delta);
		else
			countDown -= delta;

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

		int pnum = numberOfPlayers == 0 ? 4 : numberOfPlayers;
		vec2 totalPosition = vec2(0);
		
		for (int i = 0; i < pnum; i++) totalPosition += GetActors()[i]->GetPosition();
			
		cameraPosition = totalPosition / vec2(pnum);

		if (cameraType >= CameraType::Rotate) cameraRotation = GetActors()[0]->GetRotation();

		for (Actor* a : GetActors())
		{
			Car* car = dynamic_cast<Car*>(a);

			if (car)
			{
				if (car->GetLap() > laps)
				{
					MatrixRush* game = dynamic_cast<MatrixRush*>(&engine);

					if (game) game->Win(car->GetPlayerName() + "\nWon!");
				}
			}
		}

		break;
	}
}

void Track::Draw(Graphics& graphics)
{
	Scene::Draw(graphics);

	string roadWidthMessage = "Current road width - ";
	roadWidthMessage += to_string(nextRoadWidth);

	switch (state)
	{
	case StateType::Edit:

		graphics.DrawFont("roboto", "Draw your track to the screen:"
			"\nLeft Click - Place Track"
			"\nRight Click - Undo Placement"
			"\nA - Increase Road Width"
			"\nD - Decrease Road Width",
			vec2(32, graphics.GetScreenHeight() - 32));

		graphics.DrawFont("roboto", "Press to play:"
			"\n0 - Play 4 AI"
			"\n1 - Play 1 Player, 3 AI"
			"\n2 - Play 2 Player, 2 AI",
			vec2(graphics.GetScreenWidth() / 2, graphics.GetScreenHeight() - 32));

		graphics.DrawFont("roboto", roadWidthMessage.c_str(), vec2(32, 32));

		graphics.SetBackgroundColours(editBgColour[0], editBgColour[1], editBgColour[2]);

		if (coordinates.size() > 0)
		{
			if (drawMiddleLine)
			{
				DrawLine(coordinates[0], coordinates[coordinates.size() - 1],
					editLineColour[0], editLineColour[1], editLineColour[2]);

				for (int i = 1; i < coordinates.size(); i++) DrawLine(coordinates[i - 1], coordinates[i],
					editLineColour[0], editLineColour[1], editLineColour[2]);
			}

			if (drawLeftLine)
			{
				DrawLine(leftBounds[leftBounds.size() - 1], leftBounds[0],
					editLineColour[0], editLineColour[1], editLineColour[2]);

				for (int i = 1; i < leftBounds.size(); i++) DrawLine(leftBounds[i - 1], leftBounds[i],
					editLineColour[0], editLineColour[1], editLineColour[2]);
			}

			if (drawRightLine)
			{
				DrawLine(rightBounds[rightBounds.size() - 1], rightBounds[0],
					editLineColour[0], editLineColour[1], editLineColour[2]);

				for (int i = 1; i < rightBounds.size(); i++) DrawLine(rightBounds[i - 1], rightBounds[i],
					editLineColour[0], editLineColour[1], editLineColour[2]);
			}

			if (coordinates.size() > 1)
				DrawLine(leftBounds[0], rightBounds[0],
					editLineColour[0], editLineColour[1], editLineColour[2]);
		}

		break;

	case StateType::Play:

		graphics.SetBackgroundColours(0.f, 135.f / 255, 81.f / 255);


		float width = graphics.GetScreenWidth() * 2;
		float halfWidth = width / 2;

		float height = graphics.GetScreenHeight() * 2;
		float halfHeight = height / 2;


		// Draw Track //

		for (int i = 0; i < leftBounds.size(); i++)
		{
			int o = i + 1 >= leftBounds.size() ? 0 : i + 1;
			vector<vec2> poly;

			poly.push_back(leftBounds[i]);
			poly.push_back(leftBounds[o]);

			poly.push_back(rightBounds[o]);
			poly.push_back(rightBounds[i]);

			if (darks[i])
				DrawPolygon(poly, 17.f / 255, 29.f / 255, 53.f / 255);
			else
				DrawPolygon(poly, 29.f / 255, 43.f / 255, 83.f / 255);
		}

		string lapNumber = to_string(laps);


		// Draw Player 1 Info //

		if (numberOfPlayers >= 1)
		{
			// Controls Background //

			vector<vec2> controls1Back =
			{
				vec2(32, 0),
				vec2(32 + 360, 0),
				vec2(32 + 360, 328),
				vec2(32, 328),
			};

			graphics.DrawPolygon(controls1Back, 29.f / 255, 43.f / 255, 83.f / 255);


			// Controls Text //

			Car* p1 = dynamic_cast<Car*>(GetActors()[0]);

			string p1Explanation = "Lap ";
			p1Explanation += to_string(p1->GetLap()) + "/" + lapNumber +
				"\n" + p1->GetPlayerName() + " Controls:"
				"\nW - Accelerate"
				"\nA - Steer Left"
				"\nS - Reverse"
				"\nD - Seer Right"
				"\nSpace - Boost";

			graphics.DrawFont("roboto", p1Explanation.c_str(), vec2(32, 142));


			// Boost Meter //

			vector<vec2> boostMeter1back =
			{
				vec2(32, 360),
				vec2(32 + 360, 360),
				vec2(32 + 360, 328),
				vec2(32, 328),
			};

			graphics.DrawPolygon(boostMeter1back, 255.f / 255, 236.f / 255, 39.f / 255);

			if (p1->GetBoostMeter() > 0)
			{
				vector<vec2> boostMeter1 =
				{
					vec2(32, 360),
					vec2(32 + 360 * p1->GetBoostMeter(), 360),
					vec2(32 + 360 * p1->GetBoostMeter(), 328),
					vec2(32, 328),
				};
				graphics.DrawPolygon(boostMeter1, 255.f / 255, 0, 77.f / 255);
			}
		}


		// Draw Player 2 Info //

		if (numberOfPlayers >= 2)
		{
			// Controls Background //

			vector<vec2> controls2Back =
			{
				vec2(halfWidth + 32, 0),
				vec2(halfWidth + 32 + 360, 0),
				vec2(halfWidth + 32 + 360, 328),
				vec2(halfWidth + 32, 328),
			};

			graphics.DrawPolygon(controls2Back, 126.f / 255, 37.f / 255, 83.f / 255);


			// Controls Text //

			Car* p2 = dynamic_cast<Car*>(GetActors()[1]);

			string p2Explanation = "Lap ";
			p2Explanation += to_string(p2->GetLap()) + "/" + lapNumber + 
				"\n" + p2->GetPlayerName() + " Controls:"
				"\nUp - Accelerate"
				"\nLeft - Steer Left"
				"\nDown - Reverse"
				"\nRight - Seer Right"
				"\nR.Ctrl - Boost";

			graphics.DrawFont("roboto", p2Explanation.c_str(), vec2(halfWidth / 2 + 32, 142));


			// Boost Meter //

			vector<vec2> boostMeter2back =
			{
				vec2(halfWidth + 32, 360),
				vec2(halfWidth + 32 + 360, 360),
				vec2(halfWidth + 32 + 360, 328),
				vec2(halfWidth + 32, 328),
			};

			graphics.DrawPolygon(boostMeter2back, 255.f / 255, 236.f / 255, 39.f / 255);

			if (p2->GetBoostMeter() > 0)
			{
				vector<vec2> boostMeter2 =
				{
					vec2(halfWidth + 32, 360),
					vec2(halfWidth + 32 + 360 * p2->GetBoostMeter(), 360),
					vec2(halfWidth + 32 + 360 * p2->GetBoostMeter(), 328),
					vec2(halfWidth + 32, 328),
				};

				graphics.DrawPolygon(boostMeter2, 255.f / 255, 0, 77.f / 255);
			}
		}


		// Draw Finish Line //

		vector<vec2> finishLine =
		{
			vec2(leftBounds[0]),
			vec2(leftBounds[0]),
			vec2(rightBounds[0]),
			vec2(rightBounds[0]),
		};

		vec2 cornerOffset = vec2(8 * sin(angles[0]), 8 * cos(angles[0]));

		finishLine[1] += cornerOffset;
		finishLine[2] += cornerOffset;

		DrawPolygon(finishLine, 1, 0, 77.f / 255.f);


		// Draw CountDown //

		if (countDown > 0)
		{
			graphics.DrawAnimation("CountDown", graphics.GetScreenHeight() * 0.5f, 
				graphics.GetScreenHeight() * 0.5f - 128, 4.f - countDown, 1);
		}

		break;
	}
}

void Track::Init()
{
	Scene::Init();

	GetActors().clear();

	walls.clear();
	steps.clear();

	this->cameraType = CameraType::Rotate;

	nextRoadWidth = 48;

	coordinates.clear();
	widths.clear();
	angles.clear();
	darks.clear();

	leftBounds.clear();
	rightBounds.clear();

	types.clear();

	connected = false;
	drawMiddleLine = true;

	drawLeftLine = drawRightLine = dontDarkNext = false;
	numberOfPlayers = 0;

	countDown = 0;

	SetState(StateType::Edit);
}

void Track::Init(vector<Road> roads)
{
	Track::Init();

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

	if (coordinates.size() > 3)
	{
		drawLeftLine = drawRightLine = true;
		drawMiddleLine = false;
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

	if (coordinates.size() > 3) 
		
		Connect();
	else
	{
		drawMiddleLine = true;
		drawLeftLine = drawRightLine = false;
	}
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

		walls.clear();
		steps.clear();

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

		for (int i = 0; i < coordinates.size(); i++)
		{
			int o = i + 1 >= leftBounds.size() ? 0 : i + 1;

			// generate dark patches //

			float darkDistance = (2.f * widths[o]) * (2.f * widths[o]);

			float dx = coordinates[i].x - coordinates[o].x;
			float dy = coordinates[i].y - coordinates[o].y;
			float distance = dx * dx + dy * dy;

			dontDarkNext = !dontDarkNext && (darkDistance > distance);

			darks.push_back(dontDarkNext);
		}

		// set camera
		if (numberOfPlayers == 0 || numberOfPlayers == 2) SetCameraType(CameraType::None);
		else SetCameraType(CameraType::Rotate);


		// generate players //

		int l = coordinates.size() - 1;
		float startingAngle = atan(coordinates[0].x - coordinates[l].x, coordinates[0].y - coordinates[l].y);

		float startingLeftAngle = startingAngle - (pi<float>() / 2);
		int thirdWidth = widths[0] / 3;

		// generate player 1
		vec2 c1 = coordinates[0];
		c1 += vec2(thirdWidth * sin(startingLeftAngle), thirdWidth * cos(startingLeftAngle));
		c1 -= vec2(32 * sin(startingAngle), 32 * cos(startingAngle));

		if (numberOfPlayers > 0)
		{
			p1.Init(c1, -startingAngle);
			p1.SetTarget(coordinates[1]);
			AddActor(p1);
		}
		else
		{
			cp1.Init(c1, -startingAngle);
			cp1.SetTarget(coordinates[1]);
			AddActor(cp1);
		}

		// generate player 2
		vec2 c2 = coordinates[0];
		c2 -= vec2(thirdWidth * sin(startingLeftAngle), thirdWidth * cos(startingLeftAngle));
		c2 -= vec2(32 * sin(startingAngle), 32 * cos(startingAngle));

		if (numberOfPlayers > 1)
		{
			p2.Init(c2, -startingAngle);
			p2.SetTarget(coordinates[1]);
			AddActor(p2);
		}
		else
		{
			cp2.Init(c2, -startingAngle);
			cp2.SetTarget(coordinates[1]);
			AddActor(cp2);
		}

		// generate player 3
		vec2 c3 = coordinates[0];
		c3 += vec2(thirdWidth * sin(startingLeftAngle), thirdWidth * cos(startingLeftAngle));
		cp3.Init(c3, -startingAngle);

		// generate player 3
		vec2 c4 = coordinates[0];
		c4 -= vec2(thirdWidth * sin(startingLeftAngle), thirdWidth * cos(startingLeftAngle));
		cp4.Init(c4, -startingAngle);

		cp3.SetTarget(coordinates[1]);
		cp4.SetTarget(coordinates[1]);

		AddActor(cp3);
		AddActor(cp4);

		if (cameraType >= CameraType::Zoom) cameraScale = vec2(2, 2);
		countDown = 4;

		break;
	}

	this->state = state;
}

void Track::SetCameraType(CameraType cameraType)
{
	this->cameraType = cameraType;
}

int Track::GetNumberOfLaps()
{
	return laps;
}

Track::Wall::Wall(vec2 position, float rotation, int height)
	: Actor("Wall", position, rotation, 0, 2, height, false)
{
	SetRotation(GetRotation() + rotation);
	RefreshCollision();
}
