#pragma once
#include "Entity.h"
#include "Direction.h"

class Pacman :
	public Entity
{
private:
	// Animation Logic
	int animFrame = 0;
	float timer = 0;
	const float animSpeed = 200;

	// Inputs
	Uint8 keyUp;
	Uint8 keyDown;
	Uint8 keyLeft;
	Uint8 keyRight;

	// Movement Logic
	float moveTimer = 0;
	const float moveSpeed = 125;
	Direction direction = None;
	int moveStartX;
	int moveStartY;
	float drawX;
	float drawY;

	bool Move(int velocityX, int velocityY, float dt);
public:
	Pacman(GameMap* map, int x, int y, int z, Uint8 keyUp, Uint8 keyDown, Uint8 keyLeft, Uint8 keyRight);
	void Draw(SDL_Renderer* renderer, int tileSize, Spritesheet* spriteSheet) override;
	void Update(float dt, const Uint8* keys) override;
	void EventTriggered(EventType ev) override {};
	~Pacman() override;
};

