#include "Player.h"

Player::Player(float x, float y)
{
    this->positionVector = Vector2f (x,y);
    this->defaulPosVector = Vector2f (x,y);
}

Player::~Player()
{

}

void Player::up()
{
    positionVector.y -= 6;
}

void Player::down()
{
    positionVector.y += 6;
}

void Player::draw(RenderWindow * window)
{
    RectangleShape player (Vector2f(10,50));
    player.setFillColor(Color::White);
    player.setPosition(positionVector);
    window->draw(player);
}

void Player::movePlayer(float x, float y)
{
    positionVector.x = x;
    positionVector.y = y;
}

void Player::reset()
{
    positionVector = defaulPosVector;
}