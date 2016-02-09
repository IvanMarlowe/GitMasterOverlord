#include "SnakeCoords.h"

// Date constructor
SnakeCoords::SnakeCoords(float x, float y, int directions)
{
	SetSnakeCoords(x, y, directions);
}

// Date member function
void SnakeCoords::SetSnakeCoords(float x, float y, int directions)
{
	snakeX = x;
	snakeY = y;
	snakeDirections = directions;
}