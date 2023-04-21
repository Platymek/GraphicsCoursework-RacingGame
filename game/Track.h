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


private:

	bool connected, drawMiddleLine, drawLeftLine, drawRightLine, editting;

	vector<vec2> coordinates;
	vector<int> widths;
	vector<RoadType> types;

	vector<float> angles;
	vector<vec2> leftBounds;
	vector<vec2> rightBounds;

	vector<Wall> wall;

	StateType state;
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
