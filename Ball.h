#include "iostream"
#include "SFML/Graphics.hpp"
#include "stdlib.h"

using namespace sf;

class Ball
{
private:
    Vector2f positionVector;
    Vector2f movementVector;
    Vector2f defaultPosition;
    Vector2f defaultMovement;

public:
    Ball(float x, float y, float vX, float vY);
    ~Ball();
    Vector2f getPositionVector() {return positionVector;};
    Vector2f getMovementVector(){return movementVector;};
    void setPozXY(float x, float y);

    void setMovementVector(float x, float y);
    void move();
    void bounceWall();
    void bouncePad();
    void draw(RenderWindow * window);
    void reset();
};