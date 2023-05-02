#pragma once

#include "../engine/Scene.h"
#include "Wall.h"
#include <iostream>

class Track : public Scene
{
public:

	struct Road;
	class Wall;

	enum class StateType
	{
		Edit,
		Play,
		End
	};

	enum class RoadType
	{
		Normal
	};

	enum class CameraType
	{
		None,
		Track,
		Zoom,
		Rotate,
	};

	Track();

	void Process(Engine& engine, float delta) override;
	void Draw(Graphics& graphics) override;

	void Init() override;
	void Init(vector<Road> roads);
	void AddRoad(Road road, bool connect);
	void RemoveRoad();
	void Connect();

	void SetDrawMiddleLine(bool drawMiddleLine);
	void SetState(StateType state);
	void SetCameraType(CameraType cameraType);

	int GetNumberOfLaps();


private:

	bool connected, drawMiddleLine, drawLeftLine, drawRightLine, editting, dontDarkNext;

	bool won;

	int nextRoadWidth;
	vector<vec2> coordinates;
	vector<int> widths;
	vector<RoadType> types;
	vector<bool> darks;

	vector<float> angles;
	vector<vec2> leftBounds;
	vector<vec2> rightBounds;

	vector<Wall> walls;
	vector<Wall> steps;
	int laps = 3;

	StateType state;
	CameraType cameraType;
	int numberOfPlayers;

	const GLfloat editBgColour[3] = { 1.f / 255, 1.f / 255, 20.f / 255 };
	const GLfloat editLineColour[3] = { 0.f / 255, 228.f / 255, 54.f / 255 };

	float countDown;
};

struct Track::Road
{
public:

	Road();
	Road(int x, int y, int width, int type);
	Road(int x, int y, int width, RoadType type);

	int x, y, width;
	RoadType type;
};


class Track::Wall : public Actor
{
public:

	Wall(vec2 position, float rotation, int height);
};
