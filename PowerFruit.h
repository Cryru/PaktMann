#pragma once
#include "Entity.h"
class PowerFruit :
	public Entity
{
public:
	PowerFruit(int x, int y);
	~PowerFruit();
	void Update(float dt) override;
	void CollidedWith(Entity* entity) override;
	void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) override;
};

