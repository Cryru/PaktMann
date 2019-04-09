#include "SDL.h"
#include <stdio.h>

// Thanks SDL
#undef main

int main(int argc, char* args[] ) {
	printf("Starting SDL...\n");
	SDL_Init(SDL_INIT_EVERYTHING);

	printf("Creating window...\n");
	SDL_Window* win = SDL_CreateWindow("PaktMann", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 540, 0);

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
	}

	return 0;
}