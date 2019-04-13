#pragma once
#include "Entity.h"
#include "GameMap.h"

class Pacman :
	public Entity
{
private:
	// Animation Logic
	int animFrame = 0;
	float timer = 0;
	const float animSpeed = 400;

	// Input
	Uint8 keyUp;
	Uint8 keyDown;
	Uint8 keyLeft;
	Uint8 keyRight;

	// Movement Logic
	float moveTimer = 0;
	bool canMove = true;
	const float moveSpeed = 150;
	float drawX;
	float drawY;
	float moveStartX;
	float moveStartY;
	bool facingLeft = true;

public:
	Pacman(GameMap* map, int x, int y, int z, Uint8 keyUp, Uint8 keyDown, Uint8 keyLeft, Uint8 keyRight);
	void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) override;
	void Update(float dt, const Uint8* keys) override;
	~Pacman() override;
};

