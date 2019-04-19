#pragma once
#include "Entity.h"

class ScoreFruit : public Entity
{
public:
	ScoreFruit(GameMap* map, int x, int y, int z);
	~ScoreFruit() override;
	void Update(float dt, const Uint8* keys) override;
	void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) override;
	void EventTriggered(Event ev) override {};
};

