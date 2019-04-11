#include "PowerFruit.h"



PowerFruit::PowerFruit(int x, int y) : Entity(x, y)
{
}

void PowerFruit::Update(float dt)
{
}

void PowerFruit::CollidedWith(Entity* entity)
{
}

void PowerFruit::Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet)
{
	SDL_Rect loc;
	loc.x = this->x * tileSize;
	loc.y = this->y * tileSize;
	loc.w = tileSize;
	loc.h = tileSize;

	SDL_RenderCopy(renderer, spriteSheet->GetTexture(), spriteSheet->GetFrame(5), &loc);
}


PowerFruit::~PowerFruit()
{
}
