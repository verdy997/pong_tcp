#include "Game.h"

Game::Game()
{
    ball = new Ball((width/2) - 5, (height/2) - 5, 2.0, -2.0);
    p1  = new Player(0, (height / 2) - 25);
    p2  = new Player(width - 10, (height / 2) - 25);
    window.create(VideoMode(width, height), "Pong");
    window.setFramerateLimit(60);
    p1->draw(&window);
    p2->draw(&window);
    ball->draw(&window);

    font.loadFromFile("arial.ttf");
    scoreP1.setFont(font);
    scoreP1.setString(std::to_string(p1->getScore()));
    scoreP1.setCharacterSize(20);
    scoreP1.setFillColor(sf::Color::White);
    scoreP1.setPosition((width/2) - 100,0);
    window.draw(scoreP1);
    scoreP2.setFont(font);
    scoreP2.setString(std::to_string(p2->getScore()));
    scoreP2.setCharacterSize(20);
    scoreP2.setFillColor(sf::Color::White);
    scoreP2.setPosition((width/2) + 100,0);
    window.draw(scoreP2);

    window.display();

    //nadviazanie pripojenia
    std::thread connectionThread(&Game::connection, this);
    connectionThread.join();


    listening = true;
    listeningThread = std::thread(&Game::listen, this);

    Start();
}

Game::~Game()
{
    delete ball;
    delete p1;
    delete p2;
}

void Game::Start()
{
    //game loop
    while(window.isOpen()) {
        sf::Event event;
        // logika
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed){
                window.close();
            }
            if (event.type == Event::KeyPressed){
                keyInput(event.key.code);
            }
        }
        if (!pause) {
            ball->move();
            redraw();
            if (type == 1) {
                collision();
            }
            conclusion();
        }
        if(type == 1) {
            sendPacket(typePacket::BALL, ball->getPositionVector().x, ball->getPositionVector().y);
        }

        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
    listeningThread.join();
    //TODO call delete
}


void Game::connection() {
    ipAddress = IpAddress::getLocalAddress();
    int port = 29316;

    char var;
    std::cout << "c - client or s - server" << std::endl;
    std::cin >> var;

    Packet packet; // kontrolny paket
    if (var == 'c')
    {
        type = 2;
        socket.connect(ipAddress, port); // pripojenie
        std::cout << "Connected!" << std::endl;
        packet << "Hello SRV!"; // sprava nasho packetu
        if (socket.send(packet) != Socket::Done) {
            //posle packet cez socket na server, ak sa nepodari tak vypise cout
            std::cout << "Error sending packet to SRV!" << std::endl;
        }
    } else if (var == 's')
    {
        std::cout << "This instance is a server.";
        type = 1;
        //server nacuva na porte a caka na socket
        //ak nic nepride tak error ak hej tak sme napojeny
        if (listener.listen(port) != Socket::Done)
        {
            std::cout << "ERROR" << std::endl;
        } else {
            std::cout << "Client was connected" << std::endl;
        }


        if (listener.accept(socket) != Socket::Done) {
            std::cout << "Err - connection denied" << std::endl;
        }
        //snazime sa prijat spravu z packetu
        if (socket.receive(packet) != sf::Socket::Done) {
            std::cout << "Err - smtng is wrong with packet" << std::endl;
        }
        // vytiahneme spravu zo socketu a vypiseme na srv
        std::string ret;
        packet >> ret;
        std::cout << "data received: " << ret << std::endl;
    }
}

void Game::listen()
{
        Packet pack;
        while (listening){
            if (socket.receive(pack) == Socket::Done){
                openPacket(pack);
            }
        }
}

void Game::sendPacket(typePacket typ, float x, float y)
{
    Packet packet;
    packet << (int)typ << x << y;
    socket.send(packet);
}

void Game::openPacket(Packet& packet)
{
    float x,y;
    int typ;
    packet >> typ >> x >> y;
    switch (typ) {
        case 1 :
                p1->movePlayer(x,y);
            break;
        case 2 :
                p2->movePlayer(x,y);
            break;
        case 3 :
                ball->setPozXY(x,y);
            break;
        case 4 :
                p1->setScore((int)x);
                p2->setScore((int)y);
            break;
        case 5 :
                this->ball->reset();
                p1->reset();
                p2->reset();
                p1->setScore(0);
                p2->setScore(0);
                pause = false;
            break;
        default:
            std::cout << "Nothing inside packet" << std::endl;
    }
}

void Game::keyInput(Keyboard::Key key)
{
//reakcia pri klavesach W a S
    switch (key) {
        case Keyboard::W:
            if (type == 1 && p1->getPosVector().y > 0) {
                p1->up();
                sendPacket(typePacket::P1, p1->getPosVector().x, p1->getPosVector().y);
            } else if (type == 2 && p2->getPosVector().y > 0){
                p2->up();
                sendPacket(typePacket::P2, p2->getPosVector().x, p2->getPosVector().y);
            }
            break;
        case Keyboard::S:
            if (type == 1 && p1->getPosVector().y+50 < height) {
                p1->down();
                sendPacket(typePacket::P1, p1->getPosVector().x, p1->getPosVector().y);
            } else if (type == 2 && p2->getPosVector().y+50 < height){
                p2->down();
                sendPacket(typePacket::P2, p2->getPosVector().x, p2->getPosVector().y);
            }
            break;
        case Keyboard::R:
            if (pause)
            {
                sendPacket(typePacket::NEWGAME, 0.0, 0.0);
                this->ball->reset();
                p1->reset();
                p2->reset();
                p1->setScore(0);
                p2->setScore(0);
                pause = false;
            }
        default:
            break;
    }
}

void Game::redraw()
{
    //prekreslovanie
    window.clear();
    p1->draw(&window);
    p2->draw(&window);
    ball->draw(&window);
    scoreP1.setString(std::to_string(p1->getScore()));
    window.draw(scoreP1);
    scoreP2.setString(std::to_string(p2->getScore()));
    window.draw(scoreP2);
    window.display();
}

void Game::collision()
{
    Vector2f ballVector = ball->getPositionVector();
    Vector2f p1Vector = p1->getPosVector();
    Vector2f p2Vector = p2->getPosVector();

    //detekujem naraz do hornej a dolnej steny
    if (ballVector.y <= 1 || ballVector.y + 20 >= (float)height - 1)
        ball->bounceWall();

        //detekujem naraz do padov
    else if (ballVector.x <= p1Vector.x + 10 && ballVector.x >= p1Vector.x &&
             ballVector.y + 10 >= p1Vector.y && ballVector.y + 10 <= p1Vector.y + 50 && left == 0 ){
        ball->bouncePad();
        left = 1;
        right = 0;
    }

    else if  (ballVector.x + 20 >= p2Vector.x && ballVector.x <= p2Vector.x + 10 &&
              ballVector.y + 10 >= p2Vector.y && ballVector.y + 10 <= p2Vector.y + 50 && right == 0) {
        ball->bouncePad();
        left = 0;
        right = 1;
    }
        // detekujem gol
    else if (ballVector.x < -5) {
        int temp = p2->getScore();
        p2->setScore(temp+1);
        p1->reset();
        p2->reset();
        ball->reset();
        ball->setMovementVector(1.0, -2.0);
        left = 0;
        right = 0;
        if (type = 1) {
            sendPacket(typePacket::P1, p1->getPosVector().x, p1->getPosVector().y);
            sendPacket(typePacket::P2, p2->getPosVector().x, p2->getPosVector().y);
            sendPacket(typePacket::SCORE, p1->getScore(), p2->getScore());
        }
    } else if (ballVector.x > width) {
        int temp = p1->getScore();
        p1->setScore(temp+1);
        p1->reset();
        p2->reset();
        ball->reset();
        ball->setMovementVector(-1.0, 2.0);
        left = 0;
        right = 0;
        if (type = 1) {
            sendPacket(typePacket::P1, p1->getPosVector().x, p1->getPosVector().y);
            sendPacket(typePacket::P2, p2->getPosVector().x, p2->getPosVector().y);
            sendPacket(typePacket::SCORE, p1->getScore(), p2->getScore());
        }
    }

}

void Game::conclusion()
{
    if (p1->getScore() == 3) {
        pause = true;
        mutex.lock();
        endScreen("HRAC1");
        if (!pause){
            mutex.unlock();
            std::cout << "unlocknute";
        }


    } else if (p2->getScore() == 3) {
        pause = true;
        mutex.lock();
        endScreen("HRAC2");
        if (!pause){
            mutex.unlock();
            std::cout << "unlocknute";
        }

    }
}

void Game::endScreen(std::string player)
{
    window.clear();
    Text vitaz;
    vitaz.setFillColor(Color::White);
    vitaz.setString("VYHRAL "+player);
    vitaz.setCharacterSize(50);
    vitaz.setFont(font);
    vitaz.setPosition(width / 2 - 50, height / 2);
    window.draw(vitaz);
    window.display();
}