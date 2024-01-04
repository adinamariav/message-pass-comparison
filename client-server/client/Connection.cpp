//
// Created by adina on 1/4/24.
//


#include "Connection.h"


void Connection::sendMessage(const std::string& message) const {
    if (send(clientSocket, message.c_str(), message.size(), 0) == -1) {
        perror("Error sending message\n");
        exit(EXIT_FAILURE);
    }
}

std::string Connection::receiveMessage() {
    memset(buffer, 0, 1024);

    ssize_t n = recv(this->clientSocket, buffer, sizeof(buffer), 0);
    buffer[n] = '\0';

    return {buffer};
}

Connection::Connection() {
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(buffer, 0, 1024);

    // Set up the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP address
    serverAddr.sin_port = htons(8080); // Server port

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
}

int Connection::getClientSocket() const {
    return clientSocket;
}
