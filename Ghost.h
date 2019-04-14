#pragma once
#include "Entity.h"
#include "Direction.h"

class Ghost :
	public Entity
{
private:
	int sprite;

	// Movement Logic
	float moveTimer = 0;
	const float moveSpeed = 250;
	int moveStartX;
	int moveStartY;
	float drawX;
	float drawY;

	// AI
	int lastDetectedX = 0;
	int lastDetectedY = 0;
	Direction predictedDirection = None;
	int AIOffset = 0;

	// Debug
	int goalXDebug;
	int goalYDebug;
public:
	Ghost(GameMap* map, int x, int y, int z, int sprite, int aiOffset);
	~Ghost() override;
	void Update(float dt, const Uint8* keys) override;
	void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) override;
};

