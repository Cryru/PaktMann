#include "Pacman.h"
#include "GameMap.h"

Pacman::Pacman(GameMap* map, int x, int y, int z, Uint8 keyUp, Uint8 keyDown, Uint8 keyLeft, Uint8 keyRight) : Entity(map, x, y, z)
{
	this->keyUp = keyUp;
	this->keyDown = keyDown;
	this->keyLeft = keyLeft;
	this->keyRight = keyRight;

	this->drawX = x;
	this->drawY = y;
}

void Pacman::Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet)
{
	SDL_Rect loc;
	loc.x = (int)(this->drawX * tileSize);
	loc.y = (int)(this->drawY * tileSize);
	loc.w = tileSize;
	loc.h = tileSize;

	SDL_RenderCopyEx(renderer, spriteSheet->GetTexture(), spriteSheet->GetFrame(animFrame), &loc, 0,
		NULL, facingLeft ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

void Pacman::Update(float dt, const Uint8 * keys)
{
	int velocityY = 0;
	int velocityX = 0;

	// Check inputs and set velocity.
	if (keys[keyUp])
	{
		velocityY -= 1;
	}
	if (keys[keyDown])
	{
		velocityY += 1;
	}
	if (keys[keyLeft])
	{
		velocityX -= 1;
	}
	if (keys[keyRight])
	{
		velocityX += 1;
	}

	// Check if moving.
	if (moveTimer > 0)
	{
		// Lerp movement so it looks more natural.
		moveTimer += dt;
		drawX = Lerp(moveStartX, x, moveTimer / moveSpeed);
		drawY = Lerp(moveStartY, y, moveTimer / moveSpeed);
		if (moveTimer >= moveSpeed)
		{
			drawX = x;
			drawY = y;
			moveTimer = 0;
		}
	}
	// Check if should move.
	if (moveTimer == 0 && (velocityX != 0 || velocityY != 0))
	{
		moveStartX = x;
		moveStartY = y;

		// Add velocity to position, but prevent diagonal movement. Left-right is the dominant direction.
		if (velocityX != 0 && !map->GetTile(x + velocityX, y)->Solid)
		{
			x += velocityX;
		}
		else if (velocityY != 0 && !map->GetTile(x, y + velocityY)->Solid)
		{
			// Check for collision.
			y += velocityY;
		}

		// Set facing direction.
		if (velocityX < 0)
		{
			facingLeft = true;
		}
		else if (velocityX > 0)
		{
			facingLeft = false;
		}

		moveTimer = dt;
	}

	timer += dt;
	if (timer >= animSpeed)
	{
		timer -= animSpeed;

		if (animFrame == 0) animFrame = 1;
		else if (animFrame == 1) animFrame = 0;
	}
}

Pacman::~Pacman()
{

}