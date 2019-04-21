#include "Helpers.h"
#include "MapTile.h"
#include <cmath>


Helpers::Helpers()
{
}


Helpers::~Helpers()
{
}

float Helpers::Lerp(const float s, const float e, const float p)
{
	return (s + p * (e - s));
}

// Euclidean distance.
float Helpers::DistanceToCoordinate(MapTile* myPos, int x, int y)
{
	if (myPos == NULL) return 0;

	const float xDist = (float) myPos->x - x;
	const float yDist = (float) myPos->y - y;

	float dist = pow(xDist, 2) + pow(yDist, 2);
	dist = sqrt(dist);

	return dist;
}