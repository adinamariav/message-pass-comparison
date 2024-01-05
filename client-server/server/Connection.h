//
// Created by adina on 1/3/24.
//

#ifndef MESSAGE_PASS_COMPARISON_CONNECTION_H
#define MESSAGE_PASS_COMPARISON_CONNECTION_H


#include "IEpollWrapper.h"
#include <sys/socket.h>
#include <cstdint>
#include <vector>

class Connection : public IEpollWrapper {
protected:
    int descriptor;
    int id;

public:
    Connection(int socket, int id);

    Connection();

    [[nodiscard]] int getDescriptor() const override;

    [[nodiscard]] int getId() const override;

    void sendMessage(std::string message) override;

    std::string receiveMessage() override;

    std::vector<std::string> receiveMessages() override;

    ~Connection() override = default;
};


#endif //MESSAGE_PASS_COMPARISON_CONNECTION_H
