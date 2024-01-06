cmake_minimum_required(VERSION 3.13)
project(message-pass-comparison)
set(CMAKE_CXX_STANDARD 17)

add_executable(p2p main.cpp
        Peer.cpp
        Peer.h
        Connection.cpp
        Connection.h)

install(TARGETS p2p
        DESTINATION ${PROJECT_SOURCE_DIR})