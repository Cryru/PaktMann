#pragma once
class map_tile;

class helpers
{
public:
	static float lerp(float s, float e, float p);
	static float distance_to_coordinate(map_tile* my_pos, int x, int y);
};

