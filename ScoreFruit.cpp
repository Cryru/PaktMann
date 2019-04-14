#include "ScoreFruit.h"
#include "GameMap.h"

ScoreFruit::ScoreFruit(GameMap* map, int x, int y, int z) : Entity(map, x, y, z)
{
	this->type = Score;
}

void ScoreFruit::Update(float dt, const Uint8* keys)
{
}

void ScoreFruit::Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet)
{
	if(Dead) return;

	SDL_Rect loc;
	loc.x = this->x * tileSize;
	loc.y = this->y * tileSize;
	loc.w = tileSize;
	loc.h = tileSize;

	SDL_RenderCopy(renderer, spriteSheet->GetTexture(), spriteSheet->GetFrame(4), &loc);
}

ScoreFruit::~ScoreFruit()
{

}
