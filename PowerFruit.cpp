#include "PowerFruit.h"
#include "GameMap.h"

PowerFruit::PowerFruit(GameMap* map, int x, int y, int z) : Entity(map, x, y, z)
{
}

void PowerFruit::Update(float dt, const Uint8* keys)
{
}

void PowerFruit::Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet)
{
	if (Dead) return;

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
