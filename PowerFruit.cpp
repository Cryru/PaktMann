#include "PowerFruit.h"
#include "GameMap.h"

power_fruit::power_fruit(game_map* map, const int x, const int y, const int z) : entity(map, x, y, z)
{
	this->type_ = score;
}

void power_fruit::update(float dt, const Uint8* keys)
{
}

void power_fruit::draw(SDL_Renderer* renderer, const int tile_size, spritesheet* sprite_sheet)
{
	if (dead) return;

	SDL_Rect loc;
	loc.x = this->x * tile_size;
	loc.y = this->y * tile_size;
	loc.w = tile_size;
	loc.h = tile_size;

	SDL_RenderCopy(renderer, sprite_sheet->get_texture(), sprite_sheet->get_frame(5), &loc);
}

power_fruit::~power_fruit()
{
	// If the fruit died - that means it was eaten - which means the player was empowered.
	// Tell all enemies that.
	std::vector<entity*> entities = map_->get_enemies();
	for (entity* entity : entities)
	{
		entity->event_triggered(player_powered_up);
	}
}
