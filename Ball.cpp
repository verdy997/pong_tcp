#include "Ball.h"

Ball::Ball(float x, float y, float vX, float vY)
{
    this->movementVector = Vector2f (vX, vY);
    this->positionVector = Vector2f (x, y);
    this->defaultMovement = Vector2f (vX, vY);
    this->defaultPosition = Vector2f (x, y);
}

Ball::~Ball()
{

}

void Ball::bouncePad()
{
    movementVector.x = -movementVector.x;;
}

void Ball::bounceWall()
{
    movementVector.y = -movementVector.y;
}

void Ball::move()
{
    positionVector += movementVector;
}

void Ball::draw(RenderWindow *window)
{
    CircleShape ball (10.f);
    ball.setFillColor(Color::White);
    ball.setPosition(positionVector);
    window->draw(ball);
}

void Ball::setPozXY(float x, float y)
{
    this->positionVector = Vector2f (x,y);
}
void Ball::reset()
{
    positionVector = defaultPosition;
    movementVector = defaultMovement;
}

void Ball::setMovementVector(float x, float y)
{
    movementVector.x = x;
    movementVector.y = y;
}