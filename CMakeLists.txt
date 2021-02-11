cmake_minimum_required(VERSION 3.17)
project(pong)

set(SFML_STATIC_LIBRARIES TRUE)
set(CMAKE_CXX_STANDARD 14)

find_package(SFML 2.5.1 COMPONENTS system network graphics audio REQUIRED)

add_executable(pong main.cpp Game.cpp Game.h Player.cpp Player.h Ball.cpp Ball.h)
target_link_libraries(pong sfml-system sfml-network sfml-window sfml-graphics sfml-audio -static)







