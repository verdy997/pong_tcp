#include "iostream"
#include "SFML/Graphics.hpp"

using namespace sf;

class Player
{
private:
    Vector2f positionVector;
    Vector2f defaulPosVector;
    int score = 0;

public:
    Player(float x, float y);
    ~Player();
    Vector2f getPosVector(){return positionVector;};
    void movePlayer(float x, float y);
    void up();
    void down();
    void draw(RenderWindow* window);
    void reset();
    int getScore(){return score;};
    void setScore(int sc){score = sc;};
};