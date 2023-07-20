#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <random>

#include <SFML/Graphics.hpp>

#include "Paddle.h"
#include "Ball.h"

class GameLogic
{
public:
    GameLogic(){ init(); };
    ~GameLogic();
    void render();
    int launch();   // to start the game
    void init();    // basic initialization

    void drawBackground();
    void drawFigure(Paddle fig);
    void drawBall();
    void drawText();

    void moveVertical(int direction, Paddle& pad);
    void moveBall();
    void releaseBall();

    void AIControl(Paddle& pad);

    unsigned long score[2];
    int playerDirY[2];
    bool autopilot[2];  // status of AI to play the game

    bool isRunning; // game loop

    bool ballReadyForRelease;

    Paddle paddles[2];
    Ball ball;

    sf::Clock clock;
    float timer;

    sf::RenderWindow* rw;
};

#endif // !GAME_LOGIC_H