//
// Created by adina on 1/3/24.
//

#include "Connection.h"

Connection::Connection(int socket) : descriptor(socket), id(-2) {}

int Connection::getDescriptor() const {
    return descriptor;
}

int Connection::getId() const {
    return id;
}

Connection::Connection() : descriptor(0), id(0) {}


void Connection::sendMessage(std::string message)  {
    auto messageSize = static_cast<std::uint32_t>(message.size());

    if (send(descriptor, &messageSize, sizeof(messageSize), 0) == -1) {
        perror("Error sending message size");
        exit(EXIT_FAILURE);
    }

    const char* data = message.c_str();
    size_t remaining = message.size();
    ssize_t sent;

    while (remaining > 0) {
        sent = send(descriptor, data, remaining, 0);

        if (sent == -1) {
            perror("Error sending message");
            exit(EXIT_FAILURE);
        }

        data += sent;
        remaining -= static_cast<size_t>(sent);
    }
}

std::vector<std::string> Connection::receiveMessages() {
    std::vector<std::string> messages;

    while (true) {
        std::uint32_t messageSize;
        ssize_t sizeReceived = recv(this->getDescriptor(), &messageSize, sizeof(messageSize), 0);

        if (sizeReceived == -1 || sizeReceived == 0) {
            break;
        }

        std::vector<char> buffer(messageSize);
        ssize_t contentReceived = -1;

        bool isContentToRead = true;
        while (isContentToRead) {
            contentReceived = recv(this->getDescriptor(), buffer.data(), messageSize, 0);
            if (contentReceived > 0)
                isContentToRead = false;
        }

        messages.emplace_back(buffer.data(), static_cast<size_t>(contentReceived));
    }

    return messages;
}

std::string Connection::receiveMessage() {
    std::uint32_t messageSize;
    ssize_t sizeReceived = recv(this->getDescriptor(), &messageSize, sizeof(messageSize), 0);

    if (sizeReceived == -1 || sizeReceived == 0) {
        perror("Error receiving message size");
        exit(EXIT_FAILURE);
    }

    std::vector<char> buffer(messageSize);

    ssize_t contentReceived = recv(this->getDescriptor(), buffer.data(), messageSize, 0);

    if (contentReceived == -1 || contentReceived == 0) {
        perror("Error receiving message content");
        exit(EXIT_FAILURE);
    }

    return {buffer.data(), static_cast<size_t>(contentReceived)};
}

void Connection::setId(int id) {
    this->id = id;
}
