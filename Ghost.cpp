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
}

Ghost::~Ghost()
{
}

// Euclidean distance.
float DistanceToCoodinate(MapTile* myPos, int x, int y)
{
	if (myPos == NULL) return 0;

	const float xDist = myPos->x - x;
	const float yDist = myPos->y - y;

	float dist = pow(xDist, 2) + pow(yDist, 2);
	dist = sqrt(dist);

	return dist;
}

void Ghost::Update(float dt, const Uint8 * keys)
{
	Entity* player = map->GetPlayer();
	int goalX = player->x;
	int goalY = player->y;

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

	// Apply offset.
	int offsetApply = AIOffset;
	while(offsetApply != 0)
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

		if(offsetApply < 0) offsetApply += 1;
		if(offsetApply > 0) offsetApply -= 1;
	}

	goalXDebug = goalX;
	goalYDebug = goalY;

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

	float lowestDistance = 100;
	MapTile* closest = NULL;
	for (int i = 0; i < neighbors.size(); i++)
	{
		float distance = DistanceToCoodinate(neighbors[i], goalX, goalY);
		if (distance < lowestDistance)
		{
			lowestDistance = distance;
			closest = neighbors[i];
		}
	}

	if (closest != NULL)
	{
		// Check if moving.
		if (moveTimer > 0)
		{
			// Lerp movement so it looks more natural.
			moveTimer += dt;
			float p = moveTimer / moveSpeed;
			drawX = Helpers::Lerp(moveStartX, x, p);
			drawY = Helpers::Lerp(moveStartY, y, p);
			if (moveTimer >= moveSpeed)
			{
				drawX = x;
				drawY = y;
				moveTimer = 0;
			}
		}
		// Check if should move.
		if (moveTimer == 0)
		{
			int velocityX = closest->x - x;
			int velocityY = closest->y - y;

			moveStartX = x;
			moveStartY = y;
			x += velocityX;
			y += velocityY;
			moveTimer = dt;
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

	SDL_RenderCopy(renderer, spriteSheet->GetTexture(), spriteSheet->GetFrame(sprite), &loc);

	SDL_Rect predictedLoc;
	predictedLoc.x = (int)(this->goalXDebug * tileSize);
	predictedLoc.y = (int)(this->goalYDebug * tileSize);
	predictedLoc.w = tileSize;
	predictedLoc.h = tileSize;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &predictedLoc);
}
