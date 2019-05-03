#include "Ghost.h"
#include "GameMap.h"
#include "Helpers.h"
#include <set>
#include <queue>
#include <map>
#include <iostream>

ghost::ghost(game_map* map, const int x, const int y, const int z, const int sprite, const int ai_offset) : entity(map, x, y, z)
{
	this->type_ = enemy;
	this->move_start_x_ = x;
	this->move_start_y_ = y;
	this->sprite_ = sprite;
	this->ai_offset_ = ai_offset;

	this->home_x_ = x;
	this->home_y_ = y;

	this->draw_x_ = static_cast<float>(x);
	this->draw_y_ = static_cast<float>(y);
}

// Uses Euclidean distance.
map_tile* ghost::pathToGoalOld(const int x, const int y, const int goal_x, const int goal_y, game_map* map_, const float move_start_x_, const float move_start_y_, const bool afraid_)
{
	// Get neighbor non solid tiles.
	std::vector<map_tile*> neighbors;
	map_tile* left = map_->get_tile(x - 1, y);
	map_tile* right = map_->get_tile(x + 1, y);
	map_tile* top = map_->get_tile(x, y - 1);
	map_tile * bottom = map_->get_tile(x, y + 1);
	if (!left->solid && move_start_x_ != x - 1)
	{
		neighbors.push_back(left);
	}
	if (!right->solid && move_start_x_ != x + 1)
	{
		neighbors.push_back(right);
	}
	if (!top->solid && move_start_y_ != y - 1)
	{
		neighbors.push_back(top);
	}
	if (!bottom->solid && move_start_y_ != y + 1)
	{
		neighbors.push_back(bottom);
	}

	map_tile* goal_tile = nullptr;
	float best_distance = 0;
	for (map_tile* neighbor : neighbors)
	{
		const float distance = helpers::distance_to_coordinate(neighbor, goal_x, goal_y);

		// If afraid the goal tile is the one furthest from the player - run.
		if (afraid_)
		{
			if (distance > best_distance || best_distance == NULL)
			{
				best_distance = distance;
				goal_tile = neighbor;
			}
		}
		// If not afraid the goal tile is the one closes to the player - get em.
		else
		{
			if (distance < best_distance || best_distance == NULL)
			{
				best_distance = distance;
				goal_tile = neighbor;
			}
		}
	}

	return goal_tile;
}

class map_tile_wrapper
{
public:
	map_tile* tile = nullptr;
	int parent = 0;

	map_tile_wrapper()
	= default;

	map_tile_wrapper(map_tile* me, int parent = 0)
	{
		this->tile = me;
		this->parent = parent;
	}
};

// Breadth-first search
map_tile * ghost::pathToGoal(const int x, const int y, const int goal_x, const int goal_y, game_map * map_, const bool afraid_)
{
	std::map<int, map_tile_wrapper> discovered;
	std::queue<map_tile_wrapper> checkQueue;

	map_tile* startTile = map_->get_tile(x, y);
	checkQueue.push(map_tile_wrapper(startTile));
	discovered[helpers::cantor_pairing(startTile->x, startTile->y)] = startTile;
	map_tile_wrapper lastPath = checkQueue.front();

	while (!checkQueue.empty())
	{
		const map_tile_wrapper currentWrapper = checkQueue.front();
		map_tile* current = currentWrapper.tile;
		checkQueue.pop();
		lastPath = map_tile_wrapper(currentWrapper);

		// Check if the goal.
		if (current->x == goal_x && current->y == goal_y) break;

		// Get the neighbors.
		if (current->x - 1 > 0 && current->x - 1 < map_->get_width())
		{
			map_tile* neighborTile = map_->get_tile(current->x - 1, current->y);
			int tileHash = helpers::cantor_pairing(neighborTile->x, neighborTile->y);
			if (!neighborTile->solid && discovered.find(tileHash) == discovered.end())
			{
				map_tile_wrapper addedNeighbor = map_tile_wrapper(neighborTile, helpers::cantor_pairing(current->x, current->y));
				checkQueue.push(addedNeighbor);
				discovered[tileHash] = addedNeighbor;
			}
		}
		if (current->x + 1 > 0 && current->x + 1 < map_->get_width())
		{
			map_tile* neighborTile = map_->get_tile(current->x + 1, current->y);
			int tileHash = helpers::cantor_pairing(neighborTile->x, neighborTile->y);
			if (!neighborTile->solid && discovered.find(tileHash) == discovered.end())
			{
				map_tile_wrapper addedNeighbor = map_tile_wrapper(neighborTile, helpers::cantor_pairing(current->x, current->y));
				checkQueue.push(addedNeighbor);
				discovered[tileHash] = addedNeighbor;
			}
		}
		if (current->y - 1 > 0 && current->y - 1 < map_->get_height())
		{
			map_tile* neighborTile = map_->get_tile(current->x, current->y - 1);
			int tileHash = helpers::cantor_pairing(neighborTile->x, neighborTile->y);
			if (!neighborTile->solid && discovered.find(tileHash) == discovered.end())
			{
				map_tile_wrapper addedNeighbor = map_tile_wrapper(neighborTile, helpers::cantor_pairing(current->x, current->y));
				checkQueue.push(addedNeighbor);
				discovered[tileHash] = addedNeighbor;
			}
		}
		if (current->y + 1 > 0 && current->y + 1 < map_->get_height())
		{
			map_tile* neighborTile = map_->get_tile(current->x, current->y + 1);
			int tileHash = helpers::cantor_pairing(neighborTile->x, neighborTile->y);
			if (!neighborTile->solid && discovered.find(tileHash) == discovered.end())
			{
				map_tile_wrapper addedNeighbor = map_tile_wrapper(neighborTile, helpers::cantor_pairing(current->x, current->y));
				checkQueue.push(addedNeighbor);
				discovered[tileHash] = addedNeighbor;
			}
		}
	}

	if (lastPath.tile == nullptr) return nullptr;

	// Find the next one in the path before the root
	while (lastPath.parent != 0)
	{
		map_tile_wrapper nextInPath = discovered[lastPath.parent];
		if (nextInPath.parent == 0) break;
		lastPath = nextInPath;
	}

	return lastPath.tile;
}

void ghost::update(const float dt, const Uint8 * keys)
{
	entity* player = map_->get_player();

	int goal_x;
	int goal_y;

	// If going home the tile is home, otherwise it's the player.
	if (go_home_)
	{
		goal_x = home_x_;
		goal_y = home_y_;

		predicted_direction_ = none;
	}
	else
	{
		goal_x = player->x;
		goal_y = player->y;

		// Predict the direction of the player.
		if (last_detected_x_ < player->x)
		{
			predicted_direction_ = right;
			last_detected_x_ = player->x;
		}
		if (last_detected_x_ > player->x)
		{
			predicted_direction_ = left;
			last_detected_x_ = player->x;
		}
		if (last_detected_y_ < player->y)
		{
			predicted_direction_ = down;
			last_detected_y_ = player->y;
		}
		if (last_detected_y_ > player->y)
		{
			predicted_direction_ = up;
			last_detected_y_ = player->y;
		}
	}

	// Apply offset.
	int offset_apply = ai_offset_;
	while (offset_apply != 0)
	{
		bool applied = false;

		// Move in the directed prediction by the offset.
		switch (predicted_direction_)
		{
		case left:
			if (goal_x - offset_apply > 0 && goal_x - offset_apply < map_->get_width() && !map_->get_tile(goal_x - offset_apply, goal_y)->solid)
			{
				applied = true;
				goal_x -= offset_apply;
			}
			break;
		case right:
			if (goal_x + offset_apply > 0 && goal_x + offset_apply < map_->get_width() && !map_->get_tile(goal_x + offset_apply, goal_y)->solid)
			{
				applied = true;
				goal_x += offset_apply;
			}
			break;
		case up:
			if (goal_y - offset_apply > 0 && goal_y - offset_apply < map_->get_height() && !map_->get_tile(goal_x, goal_y - offset_apply)->solid)
			{
				applied = true;
				goal_y -= offset_apply;
			}
			break;
		case down:
			if (goal_y + offset_apply > 0 && goal_y + offset_apply < map_->get_height() && !map_->get_tile(goal_x, goal_y + offset_apply)->solid)
			{
				applied = true;
				goal_y += offset_apply;
			}
			break;
		default:;
		}

		if (applied) break;

		if (offset_apply < 0) offset_apply += 1;
		if (offset_apply > 0) offset_apply -= 1;
	}

	map_tile* goal_tile = nullptr;

	// If afraid use the old AI.
	if (afraid_ && !go_home_)
	{
		goal_tile = pathToGoalOld(x, y, goal_x, goal_y, map_, move_start_x_, move_start_y_, true);
	}
	else
	{
		goal_tile = pathToGoal(x, y, goal_x, goal_y, map_, afraid_ && !go_home_);
	}

	if (goal_tile != nullptr)
	{
		// Check if moving.
		if (move_timer_ > 0)
		{
			float move_speed_calc = move_speed_;
			if (afraid_) move_speed_calc = afraid_move_speed_;
			if (go_home_) move_speed_calc = go_home_move_speed_;

			// Lerp movement so it looks more natural.
			move_timer_ += dt;
			float p = move_timer_ / move_speed_calc;
			if (p > 1) p = 1;
			draw_x_ = helpers::lerp(static_cast<float>(move_start_x_), static_cast<float>(x), p);
			draw_y_ = helpers::lerp(static_cast<float>(move_start_y_), static_cast<float>(y), p);

			if (move_timer_ >= move_speed_calc)
			{
				draw_x_ = static_cast<float>(x);
				draw_y_ = static_cast<float>(y);
				move_timer_ = 0;
			}
		}
		// Check if should move.
		if (move_timer_ == 0)
		{
			const int velocity_x = goal_tile->x - x;
			const int velocity_y = goal_tile->y - y;

			move_start_x_ = x;
			move_start_y_ = y;
			x += velocity_x;
			y += velocity_y;
			move_timer_ = dt;

			// Check if should've gone home - and already have.
			if (go_home_ && x == home_x_ && y == home_y_)
			{
				afraid_ = false;
				afraid_timer_ = 0;
				go_home_ = false;
			}
		}
	}

	// Progress afraid timer - if afraid.
	if (afraid_ && !go_home_)
	{
		afraid_timer_ += dt;
		if (afraid_timer_ >= afraid_time_)
		{
			afraid_timer_ = 0;
			afraid_ = false;
		}
	}
}

void ghost::draw(SDL_Renderer * renderer, const int tile_size, spritesheet * sprite_sheet)
{
	SDL_Rect loc;
	loc.x = static_cast<int>(this->draw_x_ * tile_size);
	loc.y = static_cast<int>(this->draw_y_ * tile_size);
	loc.w = tile_size;
	loc.h = tile_size;

	int sprite_to_draw = sprite_;
	if (afraid_) sprite_to_draw = 3;
	if (go_home_) sprite_to_draw = 8;

	SDL_RenderCopy(renderer, sprite_sheet->get_texture(), sprite_sheet->get_frame(sprite_to_draw), &loc);

	loc.x = static_cast<int>(9 * tile_size);;
	loc.y = static_cast<int>(1 * tile_size);;;
}

void ghost::event_triggered(const event_type ev)
{
	if (ev == player_powered_up)
	{
		afraid_ = true;
		afraid_timer_ = 0;
	}
	else if (ev == player_is_on_your_tile)
	{
		// If the player is on my tile, and I'm not afraid of him (lol) he's dead.
		// Otherwise, i've been defeated and my go home to regenerate.
		if (!afraid_) map_->get_player()->dead = true;
		else go_home_ = true;
	}
}
