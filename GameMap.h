#pragma once
#include <vector>

#include "MapTile.h"
#include "Entity.h"
#include "GameState.h"

class game_map
{
	int width_;
	int height_;
	std::vector<std::vector<map_tile*>> map_;
	std::vector<entity*> entities_;
	entity* player_entity_;
	std::vector<entity*> score_entities_;
	std::vector<entity*> enemy_entities_;
	game_state state_ = pre_game;
public:
	/**
	* Creates a GameMap object from the specified file.
	*
	* @param map_name The file name of the text file to parse. The file is expected to be arbitrary preformatted(tm).
	* @return The loaded and ready to use map.
	*/
	static game_map* load_map(const char* map_name);
	/**
	* Create a new game map. After creation all tiles are NULL and must be manually created.
	* @param width The width of the map in tiles.
	* @param height The height of the map in tiles.
	*/
	game_map(int width, int height);
	/**
	* Get the MapTile located at the specified coordinates.
	* @param x The x axis coordinate of the tile within the map.
	* @param y The y axis coordinate of the tile within the map.
	* @returns The MapTile object located at the coordinates, or NULL if none.
	*/
	map_tile* get_tile(int x, int y);
	/**
	* Set the MapTile located at the specified coordinates.
	* @param x The x axis coordinate of the tile within the map.
	* @param y The y axis coordinate of the tile within the map.
	* @param tile_data The MapTile object to associate with the specified coordinates.
	*/
	void set_tile(int x, int y, map_tile* tile_data);
	/**
	* @returns The player entity.
	*/
	entity* get_player() const;
	/**
	* @returns Enemy entities.
	*/
	std::vector<entity*> get_enemies() const;
	/**
	* Get the width of the map in tiles.
	* @returns The width of the map in tiles.
	*/
	int get_width() const;
	/**
	* Get the height of the map in tiles.
	* @returns The height of the map in tiles.
	*/
	int get_height() const;
	/**
	* Start the game! Used to transition from the pregame press any key state to the running state.
	*/
	void start();
	/**
	* Update all entities in the map.
	* @param dt The time passed since the last update.
	* @param keys
	*/
	void update(float dt, const Uint8* keys);
	/**
	* Draw all entities in the map.
	* @param renderer The SDL renderer to use.
	* @param tile_size The size of the tiles to render.
	* @param map_spritesheet The spritesheet to use to draw the tiles.
	* @param entity_spritesheet The spritesheet to use to draw the entities.
	*/
	void draw(SDL_Renderer* renderer, int tile_size, spritesheet* map_spritesheet, spritesheet* entity_spritesheet, SDL_Texture* win_image, SDL_Texture* lose_image);
	~game_map();
	game_map(const game_map&) = delete;
	game_map& operator=(const game_map&) = delete;
	game_map(game_map&&) = delete;
	game_map& operator=(game_map&&) = delete;
};
