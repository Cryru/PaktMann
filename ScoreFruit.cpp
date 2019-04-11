#include "ScoreFruit.h"

ScoreFruit::ScoreFruit(int x, int y) : Entity(x, y)
{
}

void ScoreFruit::Update(float dt)
{
}

void ScoreFruit::CollidedWith(Entity* entity)
{
}

void ScoreFruit::Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet)
{
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
