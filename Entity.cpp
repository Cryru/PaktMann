#include "Entity.h"
#include "GameMap.h"

entity::entity(game_map* map, int x, int y, int z)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->map_ = map;
}

entity_type entity::get_type() const
{
	return this->type_;
}