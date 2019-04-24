#include "MapTile.h"

map_tile::map_tile()
{
	this->x = 0;
	this->y = 0;
	this->solid = false;
}

map_tile::map_tile(const int x, const int y, const bool solid)
{
	this->x = x;
	this->y = y;
	this->solid = solid;
}