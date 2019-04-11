#pragma once
#include "Entity.h"

class ScoreFruit : public Entity
{
public:
	ScoreFruit(int x, int y);
	~ScoreFruit();
	void Update(float dt) override;
	void CollidedWith(Entity* entity) override;
	void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) override;
};

