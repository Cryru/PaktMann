#include "Entity.h"
#include "GameMap.h"

Entity::Entity(GameMap* map, int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->map = map;
}

float Entity::Lerp(const float s, const float e, const float p)
{
	return (s + p * (e - s));
}