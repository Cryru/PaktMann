#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iostream>
#include <map>

#include "GameMap.h"
#include "Entity.h"
#include "Pacman.h"
#include "Ghost.h"
#include "ScoreFruit.h"
#include "PowerFruit.h"
#include <algorithm>

// Entity factory.
pacman* create_pacman(game_map* map, const int x, const int y)
{
	const Uint8 key_w = SDL_GetScancodeFromName("W");
	const Uint8 key_s = SDL_GetScancodeFromName("S");
	const Uint8 key_a = SDL_GetScancodeFromName("A");
	const Uint8 key_d = SDL_GetScancodeFromName("D");

	return new pacman(map, x, y, 3, key_w, key_s, key_a, key_d);
}

ghost* create_ghost_f(game_map* map, const int x, const int y)
{
	return new ghost(map, x, y, 4, 2, 5);
}

ghost* create_ghost_g(game_map* map, const int x, const  int y)
{
	return new ghost(map, x, y, 4, 6, 0);
}

ghost* create_ghost_h(game_map* map, const int x, const int y)
{
	return new ghost(map, x, y, 4, 7, -5);
}

score_fruit* create_score_fruit(game_map* map, const int x, const int y)
{
	return new score_fruit(map, x, y, 1);
}

power_fruit* create_power_fruit(game_map* map, const int x, const int y)
{
	return new power_fruit(map, x, y, 1);
}

typedef entity* (*entity_creator)(game_map* map, int, int);

std::map<char, entity_creator> entity_map = {
	{ 'p', reinterpret_cast<entity_creator>(create_pacman) },
	{ 'f', reinterpret_cast<entity_creator>(create_ghost_f) },
	{ 'g', reinterpret_cast<entity_creator>(create_ghost_g) },
	{ 'h', reinterpret_cast<entity_creator>(create_ghost_h) },
	{ '0', reinterpret_cast<entity_creator>(create_score_fruit) },
	{ 's', reinterpret_cast<entity_creator>(create_power_fruit) }
};

game_map* game_map::load_map(const char* map_name) {
	// Read the file.
	std::vector<std::string> total;
	std::string line;
	std::ifstream map_file(map_name);
	if (map_file.is_open())
	{
		while (std::getline(map_file, line))
		{
			total.push_back(line);
		}
		map_file.close();
	}
	else {
		std::cerr << "Map file " << map_name << " not found!\n";
		return nullptr;
	}

	// Verify map format
	const int map_height = total.size();
	const int map_width = map_height > 0 ? total[0].length() : 0;

	if (map_width == 0 || map_height == 0) {
		std::cerr << "Map file " << map_name << " is not properly formatted!\n";
		return nullptr;
	}

	// Construct the map.
	game_map* map = new game_map(map_width, map_height);

	for (int y = 0; y < map_height; y++)
	{
		const int line_width = total[y].length();

		// Verify map line width compared to map width.
		if (line_width != map_width) {
			std::cerr << "Map file " << map_name << " is not properly formatted on line" << y << "!\n";
			return nullptr;
		}

		for (int x = 0; x < line_width; x++)
		{
			char tile_char = total[y][x];
			map->set_tile(x, y, new map_tile(x, y, tile_char == '1'));

			// Check if creating an entity.
			if (entity_map[tile_char] != nullptr)
			{
				entity* new_entity = entity_map[tile_char](map, x, y);
				map->entities_.push_back(new_entity);

				// Add to correct entity sublist.
				switch (new_entity->get_type())
				{
				case entity_type::player:
					map->player_entity_ = new_entity;
					break;
				case entity_type::enemy:
					map->enemy_entities_.push_back(new_entity);
					break;
				case entity_type::score:
					map->score_entities_.push_back(new_entity);
					break;
				default:
					break;
				}
			}
		}
	}

	std::sort(map->entities_.begin(), map->entities_.end(), [](entity* x, entity* y) { return x->z < y->z; });

	return map;
}

game_map::game_map(const int width, const int height)
{
	this->width_ = width;
	this->height_ = height;
	this->entities_ = std::vector<entity*>();
	this->player_entity_ = nullptr;

	// Fill map with null tiles.
	for (int x = 0; x < width; x++)
	{
		map_.emplace_back();
		for (int y = 0; y < height; y++)
		{
			map_[x].emplace_back();
		}
	}
}

map_tile* game_map::get_tile(const int x, const int y)
{
	if (x > static_cast<int>(map_.size()) || y > static_cast<int>(map_[0].size()))
	{
		std::cout << "Tried to access tile " << x << ":" << y << " which doesn't exist!";
		return map_[0][0];
	}

	return map_[x][y];
}

void game_map::set_tile(const int x, const int y, map_tile* tile_data) {
	map_[x][y] = (tile_data);
}

entity* game_map::get_player() const
{
	return player_entity_;
}

std::vector<entity*> game_map::get_enemies() const
{
	return this->enemy_entities_;
}

int game_map::get_width() const
{
	return width_;
}

int game_map::get_height() const
{
	return height_;
}

void game_map::start()
{
	if (state_ != pre_game) return;
	state_ = running;
}

game_map::~game_map()
{
	player_entity_ = nullptr;
	score_entities_.clear();
	enemy_entities_.clear();

	for (std::vector<map_tile*> i : map_)
	{
		for (map_tile* p : i)
		{
			delete p;
		}
		i.clear();
	}
	map_.clear();

	for (entity* entity : entities_)
	{
		delete entity;
	}
	entities_.clear();
}

void game_map::update(const float dt, const Uint8* keys)
{
	// Check if running.
	if (state_ != running) return;

	for (entity* entity : entities_)
	{
		entity->update(dt, keys);
	}

	// Check if there is a score type entity in the player's location.
	for (size_t i = 0; i < score_entities_.size(); i++)
	{
		entity* score_entity = score_entities_[i];
		if (score_entity->x == player_entity_->x && score_entity->y == player_entity_->y)
		{
			score_entity->dead = true;
			score_entities_.erase(score_entities_.begin() + i);
			i--;
		}
	}

	// Remove dead entities.
	entities_.erase(std::remove_if(entities_.begin(), entities_.end(), [](entity* e)
		{
			if (e->dead)
			{
				delete e;
				return true;
			}
			return false;
		}), entities_.end());

	// Check if win condition - no score entities, is met.
	if (score_entities_.empty())
	{
		state_ = won;
	}

	// Check for possible lose condition - if the player is on the same tile as an enemy ask them what happens.
	for (entity* enemy_entity : enemy_entities_)
	{
		if (enemy_entity->x == player_entity_->x && enemy_entity->y == player_entity_->y)
		{
			enemy_entity->event_triggered(event_type::player_is_on_your_tile);
		}
	}

	// Check for actual lose condition - dead player.
	if (player_entity_->dead)
	{
		state_ = lost;
	}
}

void game_map::draw(SDL_Renderer* renderer, const int tile_size, spritesheet* map_spritesheet, spritesheet* entity_spritesheet, SDL_Texture* win_image, SDL_Texture* lose_image)
{
	for (int x = 0; x < width_; x++)
	{
		for (int y = 0; y < height_; y++)
		{
			map_tile* cur_tile = this->get_tile(x, y);

			SDL_Rect tile_rect;
			tile_rect.x = x * tile_size;
			tile_rect.y = y * tile_size;
			tile_rect.w = tile_size;
			tile_rect.h = tile_size;

			if (cur_tile->solid) {
				SDL_RenderCopy(renderer, map_spritesheet->get_texture(), map_spritesheet->get_frame(0), &tile_rect);
			}
			else {
				SDL_RenderCopy(renderer, map_spritesheet->get_texture(), map_spritesheet->get_frame(1), &tile_rect);
			}
		}
	}

	for (entity* entity : entities_)
	{
		entity->draw(renderer, tile_size, entity_spritesheet);
	}

	// Check if drawing lose or win.
	if (state_ != running)
	{
		if (state_ == won)
		{
			SDL_RenderCopy(renderer, win_image, nullptr, nullptr);
		}
		else if (state_ == lost)
		{
			SDL_RenderCopy(renderer, lose_image, nullptr, nullptr);
		}
	}
}