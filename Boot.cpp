#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_image.h"

#include "GameMap.h"
#include "Spritesheet.h"

// Thanks SDL
#undef main

// Config
char firstMap[] = "Assets/map.txt";
int tileSize = 16;

// Assets
Spritesheet* mapTiles;
Spritesheet* entitySheet;
SDL_Texture* winText;
SDL_Texture* loseText;

// Creates a GameMap object from the specified file.
// The file is expected to be arbitrary preformatted(tm).
GameMap* loadMap(const char* mapName) {
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
			map->SetTile(x, y, new MapTile(total[y][x] == '1'));
		}
	}

	return map;
}

// Renders the GameMap object.
void RenderMap(SDL_Renderer * renderer, GameMap * map, Spritesheet * tileMap) {
	for (size_t x = 0; x < map->GetWidth(); x++)
	{
		for (size_t y = 0; y < map->GetHeight(); y++)
		{
			MapTile* curTile = map->GetTile(x, y);

			SDL_Rect tileRect;
			tileRect.x = x * tileSize;
			tileRect.y = y * tileSize;
			tileRect.w = tileSize;
			tileRect.h = tileSize;

			if (curTile->Solid) {
				SDL_RenderCopy(renderer, tileMap->GetTexture(), mapTiles->GetFrame(0), &tileRect);
			}
			else {
				SDL_RenderCopy(renderer, tileMap->GetTexture(), mapTiles->GetFrame(1), &tileRect);
			}
		}
	}
}

int main(int argc, char* args[]) {
	printf("Starting SDL...\n");
	SDL_Init(SDL_INIT_EVERYTHING);

	printf("Creating window and graphics device...\n");
	SDL_Window* win = SDL_CreateWindow("PaktMann", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 608, SDL_WINDOW_INPUT_FOCUS);
	if (win == NULL) {
		std::cerr << "Couldn't create window, because - " << SDL_GetError();
		return 1;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		std::cerr << "Couldn't create renderer, because - " << SDL_GetError();
		return 1;
	}

	printf("Initializing SDL Image...\n");
	if (IMG_Init(IMG_INIT_PNG) <= 0)
	{
		std::cerr << "Couldn't initialize SDL Image, because - " << IMG_GetError();
		return 1;
	}

	printf("Loading assets...\n");
	mapTiles = new Spritesheet(renderer, "Assets/mapTiles.png", 16);
	entitySheet = new Spritesheet(renderer, "Assets/entitySheet.png", 16);
	winText = Spritesheet::LoadTexture(renderer, "Assets/win.png");
	loseText = Spritesheet::LoadTexture(renderer, "Assets/lose.png");

	printf("Loading map...\n");
	GameMap* map = loadMap(&firstMap[0]);

	// Set render size based on the map.
	int sizeX = tileSize * map->GetWidth();
	int sizeY = tileSize * map->GetHeight();
	SDL_RenderSetLogicalSize(renderer, sizeX, sizeY);

	printf("Starting game loop...\n");
	SDL_Event ev;
	bool isRunning = true;
	while (isRunning) {
		// Process SDL events.
		while (SDL_PollEvent(&ev) != 0) {

			// Check if no longer running.
			if (ev.type == SDL_QUIT) {
				isRunning = false;
				break;
			}
		}

		// Update input.

		// Update logic.

		// Render.
		RenderMap(renderer, map, mapTiles);

		// Swap buffers.
		SDL_RenderPresent(renderer);
	}

	// Cleanup.
	delete mapTiles;
	delete entitySheet;
	SDL_DestroyTexture(winText);
	SDL_DestroyTexture(loseText);

	delete map;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}