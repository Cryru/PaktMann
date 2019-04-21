#include "Ghost.h"
#include "GameMap.h"
#include "Helpers.h"

Ghost::Ghost(GameMap* map, int x, int y, int z, int sprite, int aiOffset) : Entity(map, x, y, z)
{
	this->type = Enemy;
	this->moveStartX = x;
	this->moveStartY = y;
	this->sprite = sprite;
	this->AIOffset = aiOffset;

	this->homeX = x;
	this->homeY = y;

	this->drawX = (float) x;
	this->drawY = (float) y;
}

Ghost::~Ghost()
{
}

void Ghost::Update(float dt, const Uint8 * keys)
{
	Entity* player = map->GetPlayer();

	int goalX = 0;
	int goalY = 0;

	// If going home the tile is home, otherwise it's the player.
	if (goHome)
	{
		goalX = homeX;
		goalY = homeY;

		predictedDirection = None;
	}
	else
	{
		goalX = player->x;
		goalY = player->y;

		// Predict the direction of the player.
		if (lastDetectedX < player->x)
		{
			predictedDirection = Right;
			lastDetectedX = player->x;
		}
		if (lastDetectedX > player->x)
		{
			predictedDirection = Left;
			lastDetectedX = player->x;
		}
		if (lastDetectedY < player->y)
		{
			predictedDirection = Down;
			lastDetectedY = player->y;
		}
		if (lastDetectedY > player->y)
		{
			predictedDirection = Up;
			lastDetectedY = player->y;
		}
	}

	// Apply offset.
	int offsetApply = AIOffset;
	while (offsetApply != 0)
	{
		bool applied = false;

		// Move in the directed prediction by the offset.
		switch (predictedDirection)
		{
		case Left:
			if (goalX - offsetApply > 0 && goalX - offsetApply < map->GetWidth() && !map->GetTile(goalX - offsetApply, goalY)->Solid)
			{
				applied = true;
				goalX -= offsetApply;
			}
			break;
		case Right:
			if (goalX + offsetApply > 0 && goalX + offsetApply < map->GetWidth() && !map->GetTile(goalX + offsetApply, goalY)->Solid)
			{
				applied = true;
				goalX += offsetApply;
			}
			break;
		case Up:
			if (goalY - offsetApply > 0 && goalY - offsetApply < map->GetHeight() && !map->GetTile(goalX, goalY - offsetApply)->Solid)
			{
				applied = true;
				goalY -= offsetApply;
			}
			break;
		case Down:
			if (goalY + offsetApply > 0 && goalY + offsetApply < map->GetHeight() && !map->GetTile(goalX, goalY + offsetApply)->Solid)
			{
				applied = true;
				goalY += offsetApply;
			}
			break;
		default:;
		}

		if (applied) break;

		if (offsetApply < 0) offsetApply += 1;
		if (offsetApply > 0) offsetApply -= 1;
	}

	// Get neighbor non solid tiles.
	std::vector<MapTile*> neighbors;
	MapTile* left = map->GetTile(x - 1, y);
	MapTile* right = map->GetTile(x + 1, y);
	MapTile* top = map->GetTile(x, y - 1);
	MapTile * bottom = map->GetTile(x, y + 1);
	if (!left->Solid && moveStartX != x - 1)
	{
		neighbors.push_back(left);
	}
	if (!right->Solid && moveStartX != x + 1)
	{
		neighbors.push_back(right);
	}
	if (!top->Solid && moveStartY != y - 1)
	{
		neighbors.push_back(top);
	}
	if (!bottom->Solid && moveStartY != y + 1)
	{
		neighbors.push_back(bottom);
	}

	MapTile* goalTile = NULL;
	float bestDistance = NULL;
	for (size_t i = 0; i < neighbors.size(); i++)
	{
		float distance = Helpers::DistanceToCoordinate(neighbors[i], goalX, goalY);

		// If afraid the goal tile is the one furthest from the player - run.
		if (afraid && !goHome)
		{
			if (distance > bestDistance || bestDistance == NULL)
			{
				bestDistance = distance;
				goalTile = neighbors[i];
			}
		}
		// If not afraid the goal tile is the one closes to the player - get em.
		else
		{
			if (distance < bestDistance || bestDistance == NULL)
			{
				bestDistance = distance;
				goalTile = neighbors[i];
			}
		}
	}

	if (goalTile != NULL)
	{
		// Check if moving.
		if (moveTimer > 0)
		{
			int moveSpeedCalc = moveSpeed;
			if (afraid) moveSpeedCalc = afraidMoveSpeed;
			if (goHome) moveSpeedCalc = goHomeMoveSpeed;

			// Lerp movement so it looks more natural.
			moveTimer += dt;
			float p = moveTimer / moveSpeedCalc;
			if(p > 1) p = 1;
			drawX = Helpers::Lerp(moveStartX, x, p);
			drawY = Helpers::Lerp(moveStartY, y, p);

			if (moveTimer >= moveSpeedCalc)
			{
				drawX = x;
				drawY = y;
				moveTimer = 0;
			}
		}
		// Check if should move.
		if (moveTimer == 0)
		{
			int velocityX = goalTile->x - x;
			int velocityY = goalTile->y - y;

			moveStartX = x;
			moveStartY = y;
			x += velocityX;
			y += velocityY;
			moveTimer = dt;

			// Check if should've gone home - and already have.
			if (goHome && x == homeX && y == homeY)
			{
				afraid = false;
				afraidTimer = 0;
				goHome = false;
			}
		}
	}

	// Progress afraid timer - if afraid.
	if (afraid && !goHome)
	{
		afraidTimer += dt;
		if (afraidTimer >= afraidTime)
		{
			afraidTimer = 0;
			afraid = false;
		}
	}
}

void Ghost::Draw(SDL_Renderer * renderer, int tileSize, Spritesheet * spriteSheet)
{
	SDL_Rect loc;
	loc.x = (int)(this->drawX * tileSize);
	loc.y = (int)(this->drawY * tileSize);
	loc.w = tileSize;
	loc.h = tileSize;

	int spriteToDraw = sprite;
	if (afraid) spriteToDraw = 3;
	if (goHome) spriteToDraw = 8;

	SDL_RenderCopy(renderer, spriteSheet->GetTexture(), spriteSheet->GetFrame(spriteToDraw), &loc);
}

void Ghost::EventTriggered(EventType ev)
{
	if (ev == PlayerPoweredUp)
	{
		afraid = true;
		afraidTimer = 0;
	}
	else if (ev == PlayerIsOnYourTile)
	{
		// If the player is on my tile, and I'm not afraid of him (lol) he's dead.
		// Otherwise, i've been defeated and my go home to regenerate.
		if (!afraid) map->GetPlayer()->Dead = true;
		else goHome = true;
	}
}
