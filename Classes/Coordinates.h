#ifndef COORDINATES_H
#define COORDINATES_H
class Coordinates {
private:
	float snakeX;
	float snakeY;
public:
	Coordinates(float x, float y);

	void SetCoordinates(float x, float y);

	float getX() { return snakeX; }
	float getY() { return snakeY; }
	void setX(float x) { snakeX = x; }
	void setY(float y) { snakeY = y; }
};
#endif