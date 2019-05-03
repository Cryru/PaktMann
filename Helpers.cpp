#include "Helpers.h"
#include "MapTile.h"
#include <cmath>

float helpers::lerp(const float s, const float e, const float p)
{
	return (s + p * (e - s));
}

// Euclidean distance.
float helpers::distance_to_coordinate(map_tile* my_pos, int x, int y)
{
	if (my_pos == NULL) return 0;

	const float xDist = (float) my_pos->x - x;
	const float yDist = (float) my_pos->y - y;

	float dist = pow(xDist, 2) + pow(yDist, 2);
	dist = sqrt(dist);

	return dist;
}

// Hashes ints.
int helpers::cantor_pairing(const int x, const int y)
{
	return static_cast<int>(0.5f * (x + y) * (x + y + 1) + y);
}