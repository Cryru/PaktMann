#pragma once
#include "Entity.h"

class Ghost :
	public Entity
{
public:
	Ghost(int x, int y);
	~Ghost();
	void Update(float dt) override;
	void CollidedWith(Entity* entity) override;
	void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) override;
};

