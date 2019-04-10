#include "GameMap.h"
#include <vector>


GameMap::GameMap(int width, int height)
{
	this->width = width;
	this->height = height;

	// Fill map with null tiles.
	for (size_t x = 0; x < width; x++)
	{ 
		map.push_back(*new std::vector<MapTile>());
		for (size_t y = 0; y < height; y++)
		{
			map[x].push_back(NULL);
		}
	}
}

MapTile* GameMap::GetTile(int x, int y)
{
	return &map[x][y];
}

void GameMap::SetTile(int x, int y, MapTile* tileData) {
	map[x][y] = (*tileData);
}

int GameMap::GetWidth()
{
	return width;
}

int GameMap::GetHeight()
{
	return height;
}

GameMap::~GameMap()
{
	map.clear();
}
