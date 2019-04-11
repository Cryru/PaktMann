#pragma once
#include "Entity.h"

class Pacman :
	public Entity
{
private:
	int animFrame = 0;
	float timer = 0;
	float animSpeed = 400;
public:
	Pacman(int x, int y);
	void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) override;
	void Update(float dt) override;
	void CollidedWith(Entity* entity) override;
	~Pacman() override;
};

