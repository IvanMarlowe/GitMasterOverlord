#pragma once
#ifndef SNAKECOORDS_H
#define SNAKECOORDS_H
class SnakeCoords {
private:
	float snakeX;
	float snakeY;
	int snakeDirections;
public:
	SnakeCoords(float x, float y, int directions);

	void SetSnakeCoords(float x, float y, int directions);

	float getX() { return snakeX; }
	float getY() { return snakeY; }
	int getDirections() { return snakeDirections; }
	void setDirection(int direction) { snakeDirections = direction; }
	void setX(float x) { snakeX = x; }
	void setY(float y) { snakeY = y; }
};
#endif