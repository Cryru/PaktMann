#include "Pacman.h"

Pacman::Pacman(int x, int y): Entity(x, y)
{
}

void Pacman::Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet)
{
	SDL_Rect loc;
	loc.x = this->x * tileSize;
	loc.y = this->y * tileSize;
	loc.w = tileSize;
	loc.h = tileSize;

	SDL_RenderCopy(renderer, spriteSheet->GetTexture(), spriteSheet->GetFrame(0), &loc);
}

void Pacman::Update(float dt)
{
}

void Pacman::CollidedWith(Entity* entity)
{
}


Pacman::~Pacman()
{
	
}