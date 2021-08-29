#pragma once
#include "Entity.h"
#include "Direction.h"

class pacman final :
	public entity
{
	// Animation Logic
	int anim_frame_ = 0;
	float timer_ = 0;
	const float anim_speed_ = 200;

	// Inputs
	Uint8 key_up_;
	Uint8 key_down_;
	Uint8 key_left_;
	Uint8 key_right_;

	// Movement Logic
	float move_timer_ = 0;
	const float move_speed_ = 125;
	direction direction_ = direction::none;
	direction direction_input_ = direction::none;
	int move_start_x_;
	int move_start_y_;
	float draw_x_;
	float draw_y_;

	bool move(int velocity_x, int velocity_y, float dt);
public:
	pacman(game_map* map, int x, int y, int z, Uint8 key_up, Uint8 key_down, Uint8 key_left, Uint8 key_right);
	void draw(SDL_Renderer* renderer, int tile_size, spritesheet* sprite_sheet) override;
	void update(float dt, const Uint8* keys) override;
	void event_triggered(event_type ev) override {};
	~pacman() override = default;
	pacman(const pacman&) = delete;
	pacman& operator=(const pacman&) = delete;
	pacman(pacman&&) = delete;
	pacman& operator=(pacman&&) = delete;
};

