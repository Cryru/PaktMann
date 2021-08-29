#pragma once
#include <SDL.h>
#include "Spritesheet.h"
#include "EntityType.h"
#include "EventType.h"

class game_map;

class entity
{
protected:
	game_map* map_ = nullptr;
	entity_type type_ = entity_type::undefined;
public:
	int x;
	int y;
	int z;
	/**
	 * Whether the entity is dead. False by default.
	 */
	bool dead = false;
	/**
	 * Create a new entity
	 * @param map The map this entity belongs to.
	 * @param x The x coordinate location of the entity.
	 * @param y The y coordinate location of the entity.
	 * @param z The z coordinate location of the entity.
	 */
	entity(game_map* map, int x, int y, int z);
	/**
	 * Update the internal entity logic.
	 * @param dt The time passed since the last edit.
	 * @param keys The pressed keys for this tick.
	 */
	virtual void update(float dt, const Uint8* keys) = 0;
	/**
	 * Render the entity.
	 * @param renderer The SDL renderer to use.
	 * @param tile_size The size of tiles within the map.
	 * @param sprite_sheet The entity spritesheet to use to render the entity. Each entity uses an index within it that it sees fit.
	 */
	virtual void draw(SDL_Renderer* renderer, int tile_size, spritesheet* sprite_sheet) = 0;
	/**
	 * Trigger an event for this entity.
	 * @param ev The event to trigger.
	 */
	virtual void event_triggered(event_type ev) = 0;
	/**
	 * @returns The type of entity this instance is.
	 */
	entity_type get_type() const;
	virtual ~entity() = default;
	entity(const entity&) = delete;
	entity& operator=(const entity&) = delete;
	entity(entity&&) = delete;
	entity& operator=(entity&&) = delete;
};

