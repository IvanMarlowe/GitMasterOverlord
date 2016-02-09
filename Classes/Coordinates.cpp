#include "Coordinates.h"

// Date constructor
Coordinates::Coordinates(float x, float y)
{
	SetCoordinates(x, y);
}

// Date member function
void Coordinates::SetCoordinates(float x, float y)
{
	snakeX = x;
	snakeY = y;
}