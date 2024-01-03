//
// Created by adina on 1/3/24.
//

#include "Connection.h"

Connection::Connection(int socket, int id) : connSocket(socket), id(id) {}

void Connection::initialize() {

}

int Connection::getSocket() const {
    return connSocket;
}

int Connection::getId() const {
    return id;
}

Connection::Connection() : connSocket(0), id(0) {}

void Connection::handle() {

}

void Connection::sendMessage(std::string message) {
    if (send(connSocket, message.c_str(), message.size(), 0) == -1) {
        perror("Error sending message\n");
        exit(EXIT_FAILURE);
    }
}

std::string Connection::receiveMessage() {
    char buf[1024];

    ssize_t n = recv(this->getSocket(), buf,sizeof(buf), 0);
    buf[n] = '\0';

    return {buf};
}
