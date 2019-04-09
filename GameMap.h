#include <stdio.h>
#include <vector>

#include "MapTile.h"

#pragma once
class GameMap
{
private:
	int width;
	int height;
	std::vector<std::vector<MapTile>>* map;
public:
	GameMap(int width, int height);
	MapTile* GetTile(int x, int y);
	void SetTile(int x, int y, MapTile* tileData);
	int GetWidth();
	int GetHeight();
	~GameMap();
};

