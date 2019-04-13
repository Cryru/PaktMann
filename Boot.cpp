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
	GameMap* map = GameMap::LoadMap(&firstMap[0]);

	// Set render size based on the map.
	int sizeX = tileSize * map->GetWidth();
	int sizeY = tileSize * map->GetHeight();
	SDL_RenderSetLogicalSize(renderer, sizeX, sizeY);

	printf("Starting game loop...\n");
	SDL_Event ev;
	bool isRunning = true;
	Uint64 before = SDL_GetPerformanceCounter();
	while (isRunning) {

		// Process SDL events.
		while (SDL_PollEvent(&ev) != 0) {

			// Check if no longer running.
			if (ev.type == SDL_QUIT) {
				isRunning = false;
				break;
			}
		}

		const Uint8* keys = SDL_GetKeyboardState(NULL);

		// Calculate delta time.
		Uint64 now = SDL_GetPerformanceCounter();
		const float deltaTime = (float)((now - before) * 1000 / (float)SDL_GetPerformanceFrequency());
		before = now; // words of wisdom.

		// Update input.

		// Update logic and render map.
		map->UpdateEntities(deltaTime, keys);
		map->Draw(renderer, tileSize, mapTiles, entitySheet);

		// Swap buffers.
		SDL_RenderPresent(renderer);
	}

	// Cleanup.
	delete mapTiles;
	delete entitySheet;
	SDL_DestroyTexture(winText);
	SDL_DestroyTexture(loseText);

	delete map;
	map = NULL;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}