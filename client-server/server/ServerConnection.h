//
// Created by adina on 1/3/24.
//

#ifndef MESSAGE_PASS_COMPARISON_SERVERCONNECTION_H
#define MESSAGE_PASS_COMPARISON_SERVERCONNECTION_H


#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include "Connection.h"

class ServerConnection : public Connection {
    sockaddr_in serverAddr{};

public:
    ServerConnection();

    ~ServerConnection() override = default;
};


#endif //MESSAGE_PASS_COMPARISON_SERVERCONNECTION_H
