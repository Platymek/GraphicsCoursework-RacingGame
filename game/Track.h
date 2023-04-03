#pragma once

#include "../engine/Scene.h"
#include <iostream>

class Track : public Scene
{
public:

	struct Road;

	enum class RoadType
	{
		Normal
	};

	Track();

	void Draw(Graphics& graphics) override;

	void Init(vector<Road> roads);
	void AddRoad(Road road, bool processFirst = true);
	void Connect();

	void SetDrawMiddleLine(bool drawMiddleLine);


private:

	bool connected, drawMiddleLine, drawLeftLine, drawRightLine;

	vector<vec2> coordinates;
	vector<int> widths;
	vector<RoadType> types;

	vector<float> angles;
	vector<vec2> leftBounds;
	vector<vec2> rightBounds;
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
