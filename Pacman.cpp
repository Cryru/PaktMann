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

	SDL_RenderCopy(renderer, spriteSheet->GetTexture(), spriteSheet->GetFrame(animFrame), &loc);
}

void Pacman::Update(float dt)
{
	timer += dt;
	if(timer >= animSpeed)
	{
		timer -= animSpeed;

		if(animFrame == 0) animFrame = 1;
		else if(animFrame == 1) animFrame = 0;
	}
}

void Pacman::CollidedWith(Entity* entity)
{
}


Pacman::~Pacman()
{
	
}