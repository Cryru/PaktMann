#pragma once
class map_tile
{
public:
	int x = 0;
	int y = 0;
	bool solid = false;
	map_tile();
	map_tile(int x, int y, bool solid);
	~map_tile() = default;
	map_tile(const map_tile&) = delete;
	map_tile& operator=(const map_tile&) = delete;
	map_tile(map_tile&&) = delete;
	map_tile& operator=(map_tile&&) = delete;
};

