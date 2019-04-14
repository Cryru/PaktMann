#pragma once
class MapTile
{
public:
	int x = 0;
	int y = 0;
	bool Solid = false;
	MapTile();
	MapTile(int x, int y, bool solid);
	~MapTile();
};

