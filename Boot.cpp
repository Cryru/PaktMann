#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_image.h"

#include "GameMap.h"

// Thanks SDL
#undef main

// Config
char firstMap[] = "Assets/map.txt";
int tileSize = 16;

// Assets
SDL_Texture* spriteSheet;
SDL_Rect solidTile;
SDL_Rect walkableTile;

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
void RenderMap(SDL_Renderer * renderer, GameMap * map, SDL_Texture * tileMap) {
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
				SDL_RenderCopy(renderer, tileMap, &solidTile, &tileRect);
			}
			else {
				SDL_RenderCopy(renderer, tileMap, &walkableTile, &tileRect);
			}
		}
	}
}

// Loads a texture from a bmp file.
SDL_Texture* LoadTexture(SDL_Renderer * renderer, const char* fileName)
{
	SDL_Surface* bmpSurface = IMG_Load(fileName);

	if (bmpSurface == NULL)
	{
		std::cerr << "Couldn't load texture " << fileName << " !\n";
		return 0;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bmpSurface);
	SDL_FreeSurface(bmpSurface);

	return texture;
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
	spriteSheet = LoadTexture(renderer, "Assets/mapTiles.png");
	solidTile.w = 16;
	solidTile.h = 16;
	walkableTile.x = 16;
	walkableTile.w = 16;
	walkableTile.h = 16;

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
		RenderMap(renderer, map, spriteSheet);

		// Swap buffers.
		SDL_RenderPresent(renderer);
	}

	// Cleanup.
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}