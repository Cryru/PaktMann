#include "Helpers.h"



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