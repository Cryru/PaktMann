#pragma once
#include <vector>

#include "MapTile.h"
#include "Entity.h"

class GameMap
{
private:
	int width;
	int height;
	std::vector<std::vector<MapTile>> map;
	std::vector<Entity*> entities;
public:
	GameMap(int width, int height);
	MapTile* GetTile(int x, int y);
	void SetTile(int x, int y, MapTile tileData);
	int GetWidth();
	int GetHeight();
	~GameMap();
	static GameMap* LoadMap(const char* mapName);
	void UpdateEntities(float dt, const Uint8* keys);
	void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* mapSpritesheet, Spritesheet* entitySpritesheet);
};