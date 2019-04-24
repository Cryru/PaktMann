#pragma once
#include "Entity.h"

class score_fruit final : public entity
{
public:
	score_fruit(game_map* map, int x, int y, int z);
	void update(float dt, const Uint8* keys) override;
	void draw(SDL_Renderer* renderer, int tile_size, spritesheet* sprite_sheet) override;
	void event_triggered(event_type ev) override;
	~score_fruit() override = default;
	score_fruit(const score_fruit&) = delete;
	score_fruit& operator=(const score_fruit&) = delete;
	score_fruit(score_fruit&&) = delete;
	score_fruit& operator=(score_fruit&&) = delete;
};

