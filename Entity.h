#pragma once
#include <SDL.h>
#include "Spritesheet.h"
#include "EntityType.h"
#include "EventType.h"

class GameMap;

class Entity
{
protected:
	GameMap* map = 0;
	EntityType type = Undefined;
public:
	int x;
	int y;
	int z;
	bool Dead = false;
	Entity(GameMap* map, int x, int y, int z);
	virtual void Update(float dt, const Uint8* keys) = 0;
	virtual void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) = 0;
	virtual void EventTriggered(EventType ev) = 0;
	virtual ~Entity() = default;
	EntityType GetType() const;
};

