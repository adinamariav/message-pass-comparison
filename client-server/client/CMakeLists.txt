cmake_minimum_required(VERSION 3.13)
project(client)

set(CMAKE_CXX_STANDARD 17)

add_executable(client main.cpp
        Connection.cpp
        Connection.h
        Client.cpp
        Client.h)