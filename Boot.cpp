#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

#include "SDL.h"

#include "GameMap.h"

// Thanks SDL
#undef main

// Config
char firstMap[] = "map.txt";

GameMap* loadMap(char* mapName) {
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

void RenderMap(SDL_Renderer * renderer, GameMap * map) {
	for (size_t x = 0; x < map->GetWidth(); x++)
	{
		for (size_t y = 0; y < map->GetHeight(); y++)
		{
			MapTile* curTile = map->GetTile(x, y);

			SDL_Rect tileRect;
			tileRect.x = x * 20;
			tileRect.y = y * 20;
			tileRect.w = 20;
			tileRect.h = 20;

			if (curTile->Solid) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			}

			SDL_RenderFillRect(renderer, &tileRect);
		}
	}
}

int main(int argc, char* args[]) {
	printf("Starting SDL...\n");
	SDL_Init(SDL_INIT_EVERYTHING);

	printf("Creating window and graphics device...\n");
	SDL_Window* win = SDL_CreateWindow("PaktMann", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, SDL_WINDOW_INPUT_FOCUS);
	if (win == NULL) {
		std::cerr << "Couldn't create window, because - " << SDL_GetError();
		return 1;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		std::cerr << "Couldn't create window, because - " << SDL_GetError();
		return 1;
	}

	SDL_Surface* screen = SDL_GetWindowSurface(win);

	printf("Loading map...\n");
	GameMap* map = loadMap(&firstMap[0]);

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
		RenderMap(renderer, map);

		// Swap buffers.
		SDL_RenderPresent(renderer);
	}

	// Cleanup.
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}