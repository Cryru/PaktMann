#pragma once
#include <SDL.h>
#include "Spritesheet.h"

class GameMap;

enum EntityType
{
	Undefined,
	Player,
	Enemy,
	Score
};

enum Event
{
	PlayerPoweredUp,
	PlayerIsOnYourTile
};

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
	virtual void EventTriggered(Event ev) = 0;
	virtual ~Entity() = default;
	static float Lerp(float s, float e, float p);
	EntityType GetType() const;
};

