#include "Spritesheet.h"
#include <SDL_image.h>
#include <iostream>

SDL_Texture* spritesheet::load_texture(SDL_Renderer* renderer, const char* file_name) {
	SDL_Surface* bmp_surface = IMG_Load(file_name);

	if (bmp_surface == nullptr)
	{
		std::cerr << "Couldn't load texture " << file_name << " !\n";
		return nullptr;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, bmp_surface);
	SDL_FreeSurface(bmp_surface);

	return texture;
}

spritesheet::spritesheet(SDL_Renderer* renderer, const char* texture_path, const int tile_size)
{
	this->texture_ = load_texture(renderer, texture_path);
	texture_width_ = 0;
	texture_height_ = 0;
	SDL_QueryTexture(this->texture_, nullptr, nullptr, &texture_width_, &texture_height_);

	const int columns = texture_width_ / tile_size;
	const int rows = texture_height_ / tile_size;

	for (int y = 0; y < rows; y++)
	{
		for (int x = 0; x < columns; x++)
		{
			SDL_Rect frame;
			frame.x = x * tile_size;
			frame.y = y * tile_size;
			frame.w = tile_size;
			frame.h = tile_size;
			this->frames_.push_back(frame);
		}
	}
}

SDL_Texture* spritesheet::get_texture() const
{
	return this->texture_;
}

SDL_Rect* spritesheet::get_frame(const int i)
{
	return &this->frames_[i];
}

spritesheet::~spritesheet()
{
	this->frames_.clear();
	SDL_DestroyTexture(this->texture_);
}
