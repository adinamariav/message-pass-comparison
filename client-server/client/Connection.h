//
// Created by adina on 1/4/24.
//

#ifndef MESSAGE_PASS_COMPARISON_CONNECTION_H
#define MESSAGE_PASS_COMPARISON_CONNECTION_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <cstring>

class Connection {
    int clientSocket;
public:
    int getClientSocket() const;

private:
    sockaddr_in serverAddr{};
    char buffer[1024] = {0};

public:
    Connection();

    void sendMessage(const std::string& message) const;
    [[nodiscard]] std::string receiveMessage();

    ~Connection() = default;
};


#endif //MESSAGE_PASS_COMPARISON_CONNECTION_H
