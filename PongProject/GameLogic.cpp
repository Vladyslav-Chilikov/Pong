#include "GameLogic.h"

GameLogic::~GameLogic()
{
    delete rw;
}

int GameLogic::launch()
{
    sf::Clock clock;
    float time;

    while (rw->isOpen())
    {
        timer = clock.getElapsedTime().asMicroseconds();
        timer = timer / 100;
        if (timer >= 100)
        {
            if (!ballReadyForRelease)
            {
                moveBall();
                ball.speed += 0.001;
            }
            clock.restart();
        }

        if (ballReadyForRelease)
        {
            ball.speed = 3.5;
            paddles[0].posY = 250;
            paddles[1].posY = 250;
        }

        render();

        if (autopilot[0])
            AIControl(paddles[0]);
        else moveVertical(playerDirY[0], paddles[0]);
        if (autopilot[1])
            AIControl(paddles[1]);
        else moveVertical(playerDirY[1], paddles[1]);

        sf::Event event;
        while (rw->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                rw->close();
            if (event.type == sf::Event::KeyPressed && isRunning)   // controls
            {
                if (event.key.code == sf::Keyboard::W) { playerDirY[0] = -1; }
                if (event.key.code == sf::Keyboard::S) { playerDirY[0] = 1; }
                if (event.key.code == sf::Keyboard::Up) { playerDirY[1] = -1; }
                if (event.key.code == sf::Keyboard::Down) { playerDirY[1] = 1; }
                if (event.key.code == sf::Keyboard::R || event.key.code == sf::Keyboard::Space) { if (ballReadyForRelease) releaseBall(); }
                if (event.key.code == sf::Keyboard::Num1) { if (autopilot[0]) autopilot[0] = false; else autopilot[0] = true; }
                if (event.key.code == sf::Keyboard::Num2) { if (autopilot[1]) autopilot[1] = false; else autopilot[1] = true; }
            }
            if (event.type == sf::Event::KeyReleased && isRunning)
            {
                if (event.key.code == sf::Keyboard::W) { playerDirY[0] = 0; }
                if (event.key.code == sf::Keyboard::S) { playerDirY[0] = 0; }
                if (event.key.code == sf::Keyboard::Up) { playerDirY[1] = 0; }
                if (event.key.code == sf::Keyboard::Down) { playerDirY[1] = 0; }
            }
        }
        if (score[0] == 9 || score[1] == 9) isRunning = false;
    }
    return 0;
}

void GameLogic::init()
{
    srand(time(NULL));
    isRunning = true;
    ballReadyForRelease = true;
    score[0] = 0;
    score[1] = 0;
    playerDirY[0] = 0;
    playerDirY[1] = 0;
    autopilot[0] = true;
    autopilot[1] = true;

    paddles[0].setPosition(100, 250);
    paddles[1].setPosition(700, 250);
    ball.setPosition(400, 250);

    rw = new sf::RenderWindow(sf::VideoMode(800, 500), "Pong");
    rw->setFramerateLimit(60);
}

void GameLogic::render()
{
    rw->clear();

    drawBackground();
    drawText();
    drawFigure(paddles[0]);
    drawFigure(paddles[1]);
    drawBall();

    rw->display();
}

void GameLogic::drawBackground()
{
    sf::RectangleShape field(sf::Vector2f(400, 500));
    sf::Color rColor(255, 90, 90, 255);
    sf::Color bColor(90, 90, 255, 255);

    field.setFillColor(rColor);
    field.setPosition(sf::Vector2f(0, 0));
    rw->draw(field);

    field.setFillColor(bColor);
    field.setPosition(sf::Vector2f(400, 0));
    rw->draw(field);
}

void GameLogic::drawFigure(Paddle fig)
{
    int padDrawDelta = 50;  // difference between mathematical and visual position of the paddle
    sf::RectangleShape pad(sf::Vector2f(10, fig.size));

    pad.setFillColor(sf::Color::White);
    pad.setOutlineThickness(2);
    pad.setOutlineColor(sf::Color::Black);

    pad.setPosition(sf::Vector2f(fig.posX, fig.posY - padDrawDelta));

    rw->draw(pad);
}

void GameLogic::drawBall()
{
    int ballDrawDelta = 12; // difference between mathematical and visual position
    sf::CircleShape ballShape((float)ball.radius);

    ballShape.setFillColor(sf::Color::White);
    ballShape.setOutlineThickness(2);
    ballShape.setOutlineColor(sf::Color::Black);

    ballShape.setPosition(sf::Vector2f(ball.posX - ballDrawDelta, ball.posY - ballDrawDelta));

    rw->draw(ballShape);
}

void GameLogic::drawText()
{
    sf::Font font;
    font.loadFromFile("assets/8bitOperatorPlus8-Bold.ttf");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(250);
    sf::Color wColor(255, 255, 255, 100);
    sf::Color bColor(0, 0, 0, 50);
    text.setFillColor(wColor);
    text.setOutlineThickness(5);
    text.setOutlineColor(bColor);

    text.setString(std::to_string(score[0]));
    text.setPosition(sf::Vector2f(200, 90));
    rw->draw(text);

    text.setString(std::to_string(score[1]));
    text.setPosition(sf::Vector2f(430, 90));
    rw->draw(text);

    text.setCharacterSize(20);
    text.setOutlineThickness(2);
    static const char* strAutoMode = "Autopilot mode : ";
    text.setString(strAutoMode);
    text.setPosition(sf::Vector2f(50, 450));
    rw->draw(text);
    text.setPosition(sf::Vector2f(500, 450));
    rw->draw(text);
    if (autopilot[0])
    {
        text.setString("ON");
    }
    else text.setString("OFF");
    text.setPosition(sf::Vector2f(265, 450));
    rw->draw(text);
    if (autopilot[1])
    {
        text.setString("ON");
    }
    else text.setString("OFF");
    text.setPosition(sf::Vector2f(715, 450));
    rw->draw(text);
}

void GameLogic::moveVertical(int direction, Paddle& pad)
{
    if (timer >= 100)
    {
        pad.posY += direction * paddles[0].speed;
        if (pad.posY < 50) pad.posY = 50;
        if (pad.posY > 450) pad.posY = 450;
        clock.restart();
    }
}

void GameLogic::releaseBall()
{

    int dirH, dirV;
    dirH = rand() % 2 + 1;
    dirV = rand() % 2 + 1;
    if (dirH > 1) ball.dirX = 1;
    else ball.dirX = -1;
    if (dirV > 1) ball.dirY = 1;
    else ball.dirY = -1;

    ballReadyForRelease = false;
}

void GameLogic::moveBall()
{
    int bounceDelta = 10;    // window of bounce for ball and paddle
    if (ball.posY < 0 + ball.radius) ball.dirY = 1;
    else if (ball.posY > 500 - ball.radius) ball.dirY = -1;
    ball.posX += ball.dirX * ball.speed;
    ball.posY += ball.dirY * ball.speed;

    if (ball.posX < 0 - ball.radius)
    {
        score[1] += 1;
        ball.setPosition(400, 250);
        ballReadyForRelease = true;
    }
    if (ball.posX > 800 + ball.radius)
    {
        score[0] += 1;
        ball.setPosition(400, 250);
        ballReadyForRelease = true;
    }
    bool bounceCondition = false;
    if (ball.posX > paddles[0].posX + ball.radius - bounceDelta && ball.posX < paddles[0].posX + ball.radius + bounceDelta)
    {
        if ((ball.posY > paddles[0].posY - paddles[0].size / 2 - ball.radius) &&
            (ball.posY < paddles[0].posY + paddles[0].size / 2 + ball.radius))
            bounceCondition = true;
        if (bounceCondition)
            ball.dirX = 1;
    }
    if (ball.posX < paddles[1].posX - 10 + ball.radius + bounceDelta && ball.posX > paddles[1].posX - 10 + ball.radius - bounceDelta)   // in this case '10' is a paddle thickness measure
    {
        if ((ball.posY > paddles[1].posY - paddles[1].size / 2 - ball.radius) &&
            (ball.posY < paddles[1].posY + paddles[1].size / 2 + ball.radius))
            bounceCondition = true;
        if (bounceCondition)
            ball.dirX = -1;
    }
}

void GameLogic::AIControl(Paddle& pad)
{
    /* It is obvious that this AI sucks
    * but
    * it was important to make it weak a little bit because without the entire 'active' check
    * Autopilot was kinda overpowered
    * To make it more hardcore it is possible to get rid of the 'active' check
    * also, screen spotting in case of hardcore version is worse
    */
    bool active = false;
    if (ball.dirX < 0 && (pad.posX == paddles[0].posX)) active = true;
    else if (ball.dirX > 0 && (pad.posX == paddles[1].posX)) active = true;

    if ((active) && (ball.dirY > 0 && (pad.posY < ball.posY)))
    {
        moveVertical(1, pad);
    }
    if ((active) && (ball.dirY < 0 && (pad.posY > ball.posY)))
    {
        moveVertical(-1, pad);
    }
}