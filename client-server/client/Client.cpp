//
// Created by adina on 1/4/24.
//

#include "Client.h"

Client::Client() {
    this->id = stoi(conn.receiveMessages().front());
}

int Client::getId() const {
    return id;
}

void Client::sendToClient(int idToSend, const std::string& content) {
    std::string message = "SEND " + std::to_string(idToSend) + " " + content;

    conn.sendMessage(message);
}

std::vector<std::string> Client::receive() {
    return conn.receiveMessages();
}

const Connection &Client::getConn() const {
    return conn;
}
