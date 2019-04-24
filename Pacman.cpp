#include "Pacman.h"
#include "GameMap.h"
#include "Helpers.h"

Pacman::Pacman(GameMap* map, int x, int y, int z, Uint8 keyUp, Uint8 keyDown, Uint8 keyLeft, Uint8 keyRight) : Entity(map, x, y, z)
{
	this->keyUp = keyUp;
	this->keyDown = keyDown;
	this->keyLeft = keyLeft;
	this->keyRight = keyRight;

	this->drawX = (float) x;
	this->drawY = (float) y;

	this->moveStartX = x;
	this->moveStartY = y;

	this->type = Player;
}

void Pacman::Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet)
{
	SDL_Rect loc;
	loc.x = (int)(this->drawX * tileSize);
	loc.y = (int)(this->drawY * tileSize);
	loc.w = tileSize;
	loc.h = tileSize;

	int angle = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	switch (direction)
	{
	case Right:
		flip = SDL_FLIP_HORIZONTAL;
		break;
	case Up:
		angle = 90;
		break;
	case Down:
		angle = -90;
		flip = SDL_FLIP_VERTICAL;
		break;
	default:;
	}

	SDL_RenderCopyEx(renderer, spriteSheet->GetTexture(), spriteSheet->GetFrame(animFrame), &loc, angle,
		NULL, flip);
}

void Pacman::Update(const float dt, const Uint8* keys)
{
	int velocityY = 0;
	int velocityX = 0;

	// Check inputs and set velocity.
	if (keys[keyUp] && !map->GetTile(x, y - 1)->Solid)
	{
		direction = Up;
	}
	if (keys[keyDown] && !map->GetTile(x, y + 1)->Solid)
	{
		direction = Down;
	}
	if (keys[keyLeft] && !map->GetTile(x - 1, y)->Solid)
	{
		direction = Left;
	}
	if (keys[keyRight] && !map->GetTile(x + 1, y)->Solid)
	{
		direction = Right;
	}

	// Check if moving.
	if (moveTimer > 0)
	{
		// Lerp movement so it looks more natural.
		moveTimer += dt;
		float p = moveTimer / moveSpeed;
		drawX = Helpers::Lerp((float) moveStartX, (float) x, p);
		drawY = Helpers::Lerp((float) moveStartY, (float) y, p);
		if (moveTimer >= moveSpeed)
		{
			drawX = (float) x;
			drawY = (float) y;
			moveTimer = 0;
		}
	}
	// Check if should move.
	if (moveTimer == 0)
	{
		switch (direction)
		{
		case Left:
			Move(-1, 0, dt);
			break;
		case Right:
			Move(1, 0, dt);
			break;
		case Up:
			Move(0, -1, dt);
			break;
		case Down:
			Move(0, 1, dt);
			break;
		default:;
		}
	}

	// Animate sprite.
	timer += dt;
	if (timer >= animSpeed)
	{
		timer -= animSpeed;

		if (animFrame == 0) animFrame = 1;
		else if (animFrame == 1) animFrame = 0;
	}
}

bool Pacman::Move(int velocityX, int velocityY, float dt)
{
	if (!map->GetTile(x + velocityX, y + velocityY)->Solid)
	{
		moveStartX = x;
		moveStartY = y;
		x += velocityX;
		y += velocityY;
		moveTimer = dt;
		return true;
	}

	return false;
}

Pacman::~Pacman()
{

}