#pragma once
#include <SDL.h>
#include "Spritesheet.h"

class Entity
{
public:
	int x;
	int y;
	Entity(int x, int y);
	virtual void Update(float dt) = 0;
	virtual void CollidedWith(Entity* entity) = 0;
	virtual void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) = 0;
	virtual ~Entity() = default;
};

