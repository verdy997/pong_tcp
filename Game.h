#include <thread>
#include "iostream"
#include "SFML/Graphics.hpp"
#include "Ball.h"
#include "Player.h"
#include "SFML/Network.hpp"

using namespace sf;

class Game
{

private:
    std::thread listeningThread;
    RenderWindow window;
    int height = 400;
    int width = 700;
    Ball * ball;
    Player * p1;
    Player * p2;
    TcpListener listener;
    TcpSocket socket;
    IpAddress ipAddress;
    int type; //1 -s , 2-c
    bool listening = false;
    int right = 0;
    int left = 0;
    Text scoreP1;
    Text scoreP2;
    Font font;
    Mutex mutex;
    bool pause = false;


public:

    enum typePacket{
        P1 = 1,
        P2 = 2,
        BALL = 3,
        SCORE = 4,
        NEWGAME = 5
    };

    Game();
    ~Game();
    void Start();
    void collision();
    void keyInput(Keyboard::Key key);
    void connection();
    void listen();
    void openPacket(Packet& packet);
    void sendPacket(typePacket typ, float x, float y);
    void redraw();
    void conclusion();
    void endScreen(std::string player);
};

