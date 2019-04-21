#pragma once
class MapTile;

class Helpers
{
public:
	Helpers();
	~Helpers();
	static float Lerp(float s, float e, float p);
	static float DistanceToCoordinate(MapTile* myPos, int x, int y);
};

