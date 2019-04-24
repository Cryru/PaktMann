#pragma once
#include "Entity.h"

class power_fruit final :
	public entity
{
public:
	power_fruit(game_map* map, int x, int y, int z);
	void update(float dt, const Uint8* keys) override;
	void draw(SDL_Renderer* renderer, int tile_size, spritesheet* sprite_sheet) override;
	void event_triggered(event_type ev) override {};
	~power_fruit() override;
	power_fruit(const power_fruit&) = delete;
	power_fruit& operator=(const power_fruit&) = delete;
	power_fruit(power_fruit&&) = delete;
	power_fruit& operator=(power_fruit&&) = delete;
};

