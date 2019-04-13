#include "Entity.h"
#include "GameMap.h"

Entity::Entity(GameMap* map, int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->map = map;
}

float Entity::Lerp(float s, float e, float p)
{
	return (s + p*(e - s));
}