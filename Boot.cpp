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
char first_map[] = "Assets/map.txt";
int tile_size = 16;

// Assets
spritesheet* mapTiles;
spritesheet* entitySheet;
SDL_Texture* winText;
SDL_Texture* loseText;

int submain()
{
	printf("Starting SDL...\n");
	SDL_Init(SDL_INIT_EVERYTHING);

	printf("Creating window and graphics device...\n");
	SDL_Window* win = SDL_CreateWindow("PaktMann", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 608, SDL_WINDOW_INPUT_FOCUS);
	if (win == nullptr) {
		std::cerr << "Couldn't create window, because - " << SDL_GetError();
		return 1;
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
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
	mapTiles = new spritesheet(renderer, "Assets/mapTiles.png", 16);
	entitySheet = new spritesheet(renderer, "Assets/entitySheet.png", 16);
	winText = spritesheet::load_texture(renderer, "Assets/win.png");
	loseText = spritesheet::load_texture(renderer, "Assets/lose.png");

	printf("Loading map...\n");
	game_map* map = game_map::load_map(&first_map[0]);

	// Set render size based on the map.
	const int size_x = tile_size * map->get_width();
	const int size_y = tile_size * map->get_height();
	SDL_RenderSetLogicalSize(renderer, size_x, size_y);

	printf("Starting game loop...\n");
	SDL_Event ev;
	bool is_running = true;
	Uint64 before = SDL_GetPerformanceCounter();
	while (is_running) {

		// Process SDL events.
		while (SDL_PollEvent(&ev) != 0) {

			// Check if no longer running.
			if (ev.type == SDL_QUIT) {
				is_running = false;
				break;
			}

			if (ev.type == SDL_KEYDOWN)
			{
				map->start();
			}
		}

		const Uint8* keys = SDL_GetKeyboardState(nullptr);

		// Calculate delta time.
		const Uint64 now = SDL_GetPerformanceCounter();
		const float delta_time = static_cast<float>((now - before) * 1000 / static_cast<float>(SDL_GetPerformanceFrequency()));
		before = now; // words of wisdom.

		// Update input.

		// Update logic and render map.
		map->update(delta_time, keys);
		map->draw(renderer, tile_size, mapTiles, entitySheet, winText, loseText);

		// Swap buffers.
		SDL_RenderPresent(renderer);
	}

	// Cleanup.
	delete mapTiles;
	delete entitySheet;
	SDL_DestroyTexture(winText);
	SDL_DestroyTexture(loseText);

	delete map;
	map = nullptr;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}

/**
 *
 * @param argc
 * @param args
 * @return
 */
int main(int argc, char* args[]) noexcept
{
	try
	{
		return submain();
	}
	catch (std::exception & e)
	{
		std::cerr << "Error occured: " << e.what();
	}
	catch (...)
	{
	}

	return 1;
}