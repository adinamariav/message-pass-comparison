//
// Created by adina on 1/6/24.
//

#include "Connection.h"


int Connection::getDescriptor() const {
    return descriptor;
}

Connection::Connection(int port) : port(port) {

    if ((this->descriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(this->descriptor, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }
    if (fcntl(this->descriptor, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        exit(EXIT_FAILURE);
    }


    int reuse = 1;
    if (setsockopt(descriptor, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Bind the descriptor to the specified address and port
    if (bind(descriptor, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    listen(descriptor, 16);
}

std::vector<std::string> Connection::receiveMessages(int peerSocket) const {
    std::vector<std::string> messages;
    bool isContentToRead = true;

    while (true) {
        std::uint32_t messageSize;
        ssize_t sizeReceived = -1;

        while (isContentToRead) {
            sizeReceived = recv(peerSocket, &messageSize, sizeof(messageSize), 0);

            if (sizeReceived != -1) {
                isContentToRead = false;
            } else {
                perror("Error");
            }
        }

        if (sizeReceived == -1 || sizeReceived == 0) {
            break;
        }

        std::vector<char> buffer(messageSize);
        ssize_t contentReceived = -1;

        isContentToRead = true;
        while (isContentToRead) {
            contentReceived = recv(peerSocket, buffer.data(), messageSize, 0);
            if (contentReceived > 0)
                isContentToRead = false;
        }

        messages.emplace_back(buffer.data(), static_cast<size_t>(contentReceived));
    }

    return messages;
}