//
// Created by adina on 1/6/24.
//

#ifndef MESSAGE_PASS_COMPARISON_CONNECTION_H
#define MESSAGE_PASS_COMPARISON_CONNECTION_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <vector>
#include <string>

class Connection {
protected:
    int descriptor;
    int port;

    sockaddr_in serverAddr{};

public:
    explicit Connection(int port);

    [[nodiscard]] int getDescriptor() const;

    std::string receiveMessage();

    [[nodiscard]] std::vector<std::string> receiveMessages(int peerSocket) const;

    ~Connection() = default;
};


#endif //MESSAGE_PASS_COMPARISON_CONNECTION_H
