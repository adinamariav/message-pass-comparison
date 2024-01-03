cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 17)

add_executable(server main.cpp
        ConnectionManager.cpp
        ConnectionManager.h
        Connection.cpp
        Connection.h
        IEpollWrapper.h
        ServerConnection.cpp
        ServerConnection.h
        Signal.cpp
        Signal.h
)