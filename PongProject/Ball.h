#ifndef BALL_H
#define BALL_H

class Ball
{
public:
	Ball() : posX(0), posY(0), speed(3.5), radius(12), dirX(0), dirY(0) {};
	void setPosition(int x, int y);
	int posX, posY;
	float speed;
	int radius;
	int dirX, dirY;
};

#endif // !BALL_H