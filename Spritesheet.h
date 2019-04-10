#pragma once
#include <SDL.h>
#include <vector>
#include <iostream>
#include <SDL_image.h>

class Spritesheet
{
private:
	std::vector<SDL_Rect> frames;
	SDL_Texture* texture;
	int textureWidth;
	int textureHeight;
public:
	Spritesheet(SDL_Renderer* renderer, const char* texturePath, int tileSize);
	~Spritesheet();
	SDL_Texture* GetTexture() const;
	SDL_Rect* GetFrame(int i);
	static SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* fileName)
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
};

