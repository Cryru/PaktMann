#pragma once
#include "Entity.h"
#include "Direction.h"

class ghost final :
	public entity
{
	int sprite_;

	// Movement Logic
	float move_timer_ = 0;
	const float move_speed_ = 250;
	int move_start_x_;
	int move_start_y_;
	float draw_x_;
	float draw_y_;

	// AI
	int last_detected_x_ = 0;
	int last_detected_y_ = 0;
	direction predicted_direction_ = none;
	int ai_offset_ = 0;

	// Afraid AI and Logic
	bool afraid_ = false;
	float afraid_time_ = 5000;
	float afraid_timer_ = 0;
	float afraid_move_speed_ = 400;
	bool go_home_ = false;
	float go_home_move_speed_ = 500;
	int home_x_;
	int home_y_;
public:
	ghost(game_map* map, int x, int y, int z, int sprite, int ai_offset);
	void update(float dt, const Uint8* keys) override;
	void draw(SDL_Renderer* renderer, int tile_size, spritesheet* sprite_sheet) override;
	void event_triggered(event_type ev) override;
	~ghost() override = default;
	ghost(const ghost&) = delete;
	ghost& operator=(const ghost&) = delete;
	ghost(ghost&&) = delete;
	ghost& operator=(ghost&&) = delete;
};

