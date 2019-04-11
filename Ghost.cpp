#include "Ghost.h"

Ghost::Ghost(int x, int y) : Entity(x, y)
{
}

Ghost::~Ghost()
{
}

void Ghost::Update(float dt)
{
}

void Ghost::CollidedWith(Entity* entity)
{
}

void Ghost::Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet)
{
	SDL_Rect loc;
	loc.x = this->x * tileSize;
	loc.y = this->y * tileSize;
	loc.w = tileSize;
	loc.h = tileSize;

	SDL_RenderCopy(renderer, spriteSheet->GetTexture(), spriteSheet->GetFrame(2), &loc);
}
