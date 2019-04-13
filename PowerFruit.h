#pragma once
#include "Entity.h"
#include "GameMap.h"

class PowerFruit :
	public Entity
{
public:
	PowerFruit(GameMap* map, int x, int y, int z);
	~PowerFruit() override;
	void Update(float dt, const Uint8* keys) override;
	void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) override;
};

