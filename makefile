pong: Ball.o Player.o
    g++ Ball.o Player.o -o pong


Ball.o: Ball.cpp
    g++ -c Ball.cpp

Player.o: Player.cpp
    g++ -c Player.cpp


clean:
    rm *.o pong