#include "Pacman.h"
#include "GameMap.h"
#include "Helpers.h"

pacman::pacman(game_map* map, const int x, const int y, const int z, const Uint8 key_up, const Uint8 key_down, const Uint8 key_left, const Uint8 key_right) : entity(map, x, y, z)
{
	this->key_up_ = key_up;
	this->key_down_ = key_down;
	this->key_left_ = key_left;
	this->key_right_ = key_right;

	this->draw_x_ = static_cast<float>(x);
	this->draw_y_ = static_cast<float>(y);

	this->move_start_x_ = x;
	this->move_start_y_ = y;

	this->type_ = player;
}

void pacman::draw(SDL_Renderer* renderer, const int tile_size, spritesheet* sprite_sheet)
{
	SDL_Rect loc;
	loc.x = static_cast<int>(this->draw_x_ * tile_size);
	loc.y = static_cast<int>(this->draw_y_ * tile_size);
	loc.w = tile_size;
	loc.h = tile_size;

	int angle = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	switch (direction_)
	{
	case right:
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case up:
		angle = 90;
		break;
	case down:
		angle = -90;
		flip = SDL_FLIP_VERTICAL;
		break;
	default:;
	}

	SDL_RenderCopyEx(renderer, sprite_sheet->get_texture(), sprite_sheet->get_frame(anim_frame_), &loc, angle,
		nullptr, flip);
}

void pacman::update(const float dt, const Uint8* keys)
{
	// Check inputs and set velocity.
	if (keys[key_up_] && !map_->get_tile(x, y - 1)->solid)
	{
		direction_ = up;
	}
	if (keys[key_down_] && !map_->get_tile(x, y + 1)->solid)
	{
		direction_ = down;
	}
	if (keys[key_left_] && !map_->get_tile(x - 1, y)->solid)
	{
		direction_ = left;
	}
	if (keys[key_right_] && !map_->get_tile(x + 1, y)->solid)
	{
		direction_ = right;
	}

	// Check if moving.
	if (move_timer_ > 0)
	{
		// Lerp movement so it looks more natural.
		move_timer_ += dt;
		const float p = move_timer_ / move_speed_;
		draw_x_ = helpers::lerp(static_cast<float>(move_start_x_), static_cast<float>(x), p);
		draw_y_ = helpers::lerp(static_cast<float>(move_start_y_), static_cast<float>(y), p);
		if (move_timer_ >= move_speed_)
		{
			draw_x_ = static_cast<float>(x);
			draw_y_ = static_cast<float>(y);
			move_timer_ = 0;
		}
	}
	// Check if should move.
	if (move_timer_ == 0)
	{
		switch (direction_)
		{
		case left:
			move(-1, 0, dt);
			break;
		case right:
			move(1, 0, dt);
			break;
		case up:
			move(0, -1, dt);
			break;
		case down:
			move(0, 1, dt);
			break;
		default:;
		}
	}

	// Animate sprite.
	timer_ += dt;
	if (timer_ >= anim_speed_)
	{
		timer_ -= anim_speed_;

		if (anim_frame_ == 0) anim_frame_ = 1;
		else if (anim_frame_ == 1) anim_frame_ = 0;
	}
}

bool pacman::move(const int velocity_x, const int velocity_y, const float dt)
{
	if (!map_->get_tile(x + velocity_x, y + velocity_y)->solid)
	{
		move_start_x_ = x;
		move_start_y_ = y;
		x += velocity_x;
		y += velocity_y;
		move_timer_ = dt;
		return true;
	}

	return false;
}