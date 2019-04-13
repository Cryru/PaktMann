#pragma once
#include "Entity.h"

class Ghost :
	public Entity
{
public:
	Ghost(GameMap* map, int x, int y, int z);
	~Ghost() override;
	void Update(float dt, const Uint8* keys) override;
	void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) override;
};

