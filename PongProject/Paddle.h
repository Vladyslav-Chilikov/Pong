#ifndef PADDLE_H
#define PADDLE_H

class Paddle
{
public:
	Paddle(float sz = 100.0, float spd = 4) : size(sz), speed(spd), posX(0), posY(0) {};
	void setPosition(int x, int y);
	int posX, posY;
	float size;
	float speed;
};

#endif // !PADDLE_H