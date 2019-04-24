#include "ScoreFruit.h"
#include "GameMap.h"

score_fruit::score_fruit(game_map* map, int x, int y, int z) : entity(map, x, y, z)
{
	this->type_ = score;
}

void score_fruit::update(float dt, const Uint8* keys)
{
}

void score_fruit::draw(SDL_Renderer* renderer, int tile_size, spritesheet* sprite_sheet)
{
	if (dead) return;

	SDL_Rect loc;
	loc.x = this->x * tile_size;
	loc.y = this->y * tile_size;
	loc.w = tile_size;
	loc.h = tile_size;

	SDL_RenderCopy(renderer, sprite_sheet->get_texture(), sprite_sheet->get_frame(4), &loc);
}

void score_fruit::event_triggered(event_type ev)
{
}