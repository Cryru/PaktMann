#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <map>

#include "GameMap.h"
#include "Entity.h"
#include "Pacman.h"
#include "Ghost.h"
#include "ScoreFruit.h"
#include "PowerFruit.h"
#include <algorithm>

// Entity factory.
Pacman* createPacman(GameMap* map, int x, int y)
{
	Uint8 keyW = SDL_GetScancodeFromName("W");
	Uint8 keyS = SDL_GetScancodeFromName("S");
	Uint8 keyA = SDL_GetScancodeFromName("A");
	Uint8 keyD = SDL_GetScancodeFromName("D");

	return new Pacman(map, x, y, 3, keyW, keyS, keyA, keyD);
}

Ghost* createGhost(GameMap* map, int x, int y)
{
	return new Ghost(map, x, y, 2);
}

ScoreFruit* createScoreFruit(GameMap* map, int x, int y)
{
	return new ScoreFruit(map, x, y, 1);
}

PowerFruit* createPowerFruit(GameMap* map, int x, int y)
{
	return new PowerFruit(map, x, y, 1);
}

typedef Entity* (*entityCreator)(GameMap* map, int, int);

std::map<char, entityCreator> entityMap = {
	{ 'p', (entityCreator)createPacman },
	{ 'g', (entityCreator)createGhost },
	{ '0', (entityCreator)createScoreFruit },
	{ 's', (entityCreator)createPowerFruit }
};

/**
 * Creates a GameMap object from the specified file.
 *
 * @param mapName The file name of the text file to parse. The file is expected to be arbitrary preformatted(tm).
 * @return The loaded and ready to use map.
 */
GameMap* GameMap::LoadMap(const char* mapName) {
	// Read the file.
	std::vector<std::string> total;
	std::string line;
	std::ifstream mapFile(mapName);
	if (mapFile.is_open())
	{
		while (std::getline(mapFile, line))
		{
			total.push_back(line);
		}
		mapFile.close();
	}
	else {
		std::cerr << "Map file " << mapName << " not found!\n";
		return NULL;
	}

	// Verify map format
	int mapHeight = total.size();
	int mapWidth = mapHeight > 0 ? total[0].length() : 0;

	if (mapWidth == 0 || mapHeight == 0) {
		std::cerr << "Map file " << mapName << " is not properly formatted!\n";
		return NULL;
	}

	// Construct the map.
	GameMap* map = new GameMap(mapWidth, mapHeight);

	for (size_t y = 0; y < mapHeight; y++)
	{
		int lineWidth = total[y].length();

		// Verify map line width compared to map width.
		if (lineWidth != mapWidth) {
			std::cerr << "Map file " << mapName << " is not properly formatted on line" << y << "!\n";
			return NULL;
		}

		for (size_t x = 0; x < lineWidth; x++)
		{
			char tileChar = total[y][x];
			map->SetTile(x, y, MapTile(tileChar == '1'));

			// Check if creating an entity.
			if (entityMap[tileChar] != NULL)
			{
				Entity* newEntity = entityMap[tileChar](map, x, y);
				map->entities.push_back(newEntity);
			}
		}
	}

	std::sort(map->entities.begin(), map->entities.end(), [](Entity* x, Entity* y) { return x->z < y->z; });

	return map;
}

/**
 * Create a new game map. After creation all tiles are NULL and must be manually created.
 * @param width The width of the map in tiles.
 * @param height The height of the map in tiles.
 */
GameMap::GameMap(int width, int height)
{
	this->width = width;
	this->height = height;
	this->entities = std::vector<Entity*>();

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

/**
 * Get the MapTile located at the specified coordinates.
 * @param x The x axis coordinate of the tile within the map.
 * @param y The y axis coordinate of the tile within the map.
 * @returns The MapTile object located at the coordinates, or NULL if none.
 */
MapTile* GameMap::GetTile(int x, int y)
{
	return &map[x][y];
}

/**
 * Set the MapTile located at the specified coordinates.
 * @param x The x axis coordinate of the tile within the map.
 * @param y The y axis coordinate of the tile within the map.
 * @param tileData The MapTile object to associate with the specified coordinates.
 */
void GameMap::SetTile(int x, int y, MapTile tileData) {
	map[x][y] = (tileData);
}

/**
 * Get the width of the map in tiles.
 * @returns The width of the map in tiles.
 */
int GameMap::GetWidth()
{
	return width;
}

/**
 * Get the height of the map in tiles.
 * @returns The height of the map in tiles.
 */
int GameMap::GetHeight()
{
	return height;
}

GameMap::~GameMap()
{
	map.clear();
	for (size_t i = 0; i < entities.size(); i++)
	{
		delete entities[i];
	}
	entities.clear();
}

/**
 * Update all entities in the map.
 * @param dt The time passed since the last update.
 */
void GameMap::UpdateEntities(float dt, const Uint8* keys)
{
	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->Update(dt, keys);
	}

	// Remove dead entities.
	entities.erase(std::remove_if(entities.begin(), entities.end(), [](Entity* e) { return e->Dead; }), entities.end());
}

/**
 * Draw all entities in the map.
 * @param renderer The SDL renderer to use.
 * @param tileSize The size of the tiles to render.
 * @param mapSpritesheet The spritesheet to use to draw the tiles.
 * @param entitySpritesheet The spritesheet to use to draw the entities.
 */
void GameMap::Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* mapSpritesheet, Spritesheet* entitySpritesheet)
{
	for (size_t x = 0; x < width; x++)
	{
		for (size_t y = 0; y < height; y++)
		{
			MapTile* curTile = this->GetTile(x, y);

			SDL_Rect tileRect;
			tileRect.x = x * tileSize;
			tileRect.y = y * tileSize;
			tileRect.w = tileSize;
			tileRect.h = tileSize;

			if (curTile->Solid) {
				SDL_RenderCopy(renderer, mapSpritesheet->GetTexture(), mapSpritesheet->GetFrame(0), &tileRect);
			}
			else {
				SDL_RenderCopy(renderer, mapSpritesheet->GetTexture(), mapSpritesheet->GetFrame(1), &tileRect);
			}
		}
	}

	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->Draw(renderer, tileSize, entitySpritesheet);
	}
}