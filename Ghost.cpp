#include "Ghost.h"
#include "GameMap.h"

Ghost::Ghost(GameMap* map, int x, int y, int z) : Entity(map, x, y, z)
{
	this->type = Enemy;
}

Ghost::~Ghost()
{
}

void Ghost::Update(float dt, const Uint8* keys)
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
