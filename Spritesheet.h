#pragma once
#include <SDL.h>
#include <vector>

class spritesheet
{
	std::vector<SDL_Rect> frames_;
	SDL_Texture* texture_;
	int texture_width_;
	int texture_height_;
public:
	spritesheet(SDL_Renderer* renderer, const char* texture_path, int tile_size);
	SDL_Texture* get_texture() const;
	SDL_Rect* get_frame(int i);
	static SDL_Texture* load_texture(SDL_Renderer* renderer, const char* file_name);
	~spritesheet();
	spritesheet(const spritesheet&) = delete;
	spritesheet& operator=(const spritesheet&) = delete;
	spritesheet(spritesheet&&) = delete;
	spritesheet& operator=(spritesheet&&) = delete;
};

