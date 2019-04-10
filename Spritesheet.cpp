#include "Spritesheet.h"
#include <iostream>
#include <SDL_image.h>

Spritesheet::Spritesheet(SDL_Renderer* renderer, const char* texturePath, int tileSize)
{
	this->texture = LoadTexture(renderer, texturePath);
	textureWidth = 0;
	textureHeight = 0;
	SDL_QueryTexture(this->texture, NULL, NULL, &textureWidth, &textureHeight);

	int columns = textureWidth / tileSize;
	int rows = textureHeight / tileSize;

	for (size_t y = 0; y < rows; y++)
	{
		for (size_t x = 0; x < columns; x++)
		{
			SDL_Rect frame;
			frame.x = x * tileSize;
			frame.y = y * tileSize;
			frame.w = tileSize;
			frame.h = tileSize;
			this->frames.push_back(frame);
		}
	}
}

SDL_Texture* Spritesheet::GetTexture() const
{
	return this->texture;
}

SDL_Rect* Spritesheet::GetFrame(int i)
{
	return &this->frames[i];
}

Spritesheet::~Spritesheet()
{
	this->frames.clear();
	SDL_DestroyTexture(this->texture);
}
