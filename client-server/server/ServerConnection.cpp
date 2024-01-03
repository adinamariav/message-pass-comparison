//
// Created by adina on 1/3/24.
//

#include "ServerConnection.h"

ServerConnection::ServerConnection() {
    this->id = 0;

    if ((this->connSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int reuse = 1;
    if (setsockopt(connSocket, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    // Bind the connSocket to the specified address and port
    if (bind(connSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(connSocket, 16);
}

void ServerConnection::initialize() {
    Connection::initialize();



}

void ServerConnection::handle() {
    Connection::handle();

}
