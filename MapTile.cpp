#include "MapTile.h"

MapTile::MapTile()
{
	this->x = 0;
	this->y = 0;
	this->Solid = false;
}

MapTile::MapTile(int x, int y, bool solid)
{
	this->x = x;
	this->y = y;
	this->Solid = solid;
}


MapTile::~MapTile()
{

}
