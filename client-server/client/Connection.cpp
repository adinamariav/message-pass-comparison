//
// Created by adina on 1/4/24.
//


#include "Connection.h"



void Connection::sendMessage(const std::string& message) const {
    auto messageSize = static_cast<std::uint32_t>(message.size());

    if (send(clientSocket, &messageSize, sizeof(messageSize), 0) == -1) {
        perror("Error sending message size");
        exit(EXIT_FAILURE);
    }

    const char* data = message.c_str();
    size_t remaining = message.size();
    ssize_t sent;

    while (remaining > 0) {
        sent = send(clientSocket, data, remaining, 0);

        if (sent == -1) {
            perror("Error sending message");
            exit(EXIT_FAILURE);
        }

        data += sent;
        remaining -= static_cast<size_t>(sent);
    }
}

std::string Connection::receiveMessage() const {
    std::uint32_t messageSize;
    ssize_t sizeReceived = recv(this->clientSocket, &messageSize, sizeof(messageSize), 0);

    if (sizeReceived == -1 || sizeReceived == 0) {
        perror("Error receiving message size");
        exit(EXIT_FAILURE);
    }

    std::vector<char> buffer(messageSize);

    ssize_t contentReceived = recv(this->clientSocket, buffer.data(), messageSize, 0);

    if (contentReceived == -1 || contentReceived == 0) {
        perror("Error receiving message content");
        exit(EXIT_FAILURE);
    }

    return {buffer.data(), static_cast<size_t>(contentReceived)};
}

std::vector<std::string> Connection::receiveMessages() const {
    std::vector<std::string> messages;
    bool isContentToRead = true;

    while (true) {
        std::uint32_t messageSize;
        ssize_t sizeReceived = -1;


        while (isContentToRead) {
            sizeReceived = recv(this->clientSocket, &messageSize, sizeof(messageSize), 0);

            if (sizeReceived != -1) {
                isContentToRead = false;
            }
        }

        if (sizeReceived == -1 || sizeReceived == 0) {
            break;
        }


        std::vector<char> buffer(messageSize);
        ssize_t contentReceived = -1;
        isContentToRead = true;

        while (isContentToRead) {
            contentReceived = recv(this->clientSocket, buffer.data(), messageSize, 0);
            if (contentReceived == messageSize)
                isContentToRead = false;
        }

        messages.emplace_back(buffer.data(), static_cast<size_t>(contentReceived));
    }

    return messages;
}

Connection::Connection() {
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(buffer, 0, 1024);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(8080);

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    int flags = fcntl(clientSocket, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
    if (fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
}

int Connection::getClientSocket() const {
    return clientSocket;
}
