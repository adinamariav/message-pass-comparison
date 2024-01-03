//
// Created by adina on 1/3/24.
//

#ifndef MESSAGE_PASS_COMPARISON_CONNECTION_H
#define MESSAGE_PASS_COMPARISON_CONNECTION_H


#include "IConnection.h"
#include <sys/socket.h>

class Connection : public IConnection {
protected:
    int connSocket;
    int id;

public:
    Connection(int socket, int id);

    Connection();

    [[nodiscard]] int getSocket() const override;

    [[nodiscard]] int getId() const override;

    void initialize() override;

    void handle() override;

    void sendMessage(std::string message) override;
    std::string receiveMessage() override;

    ~Connection() override = default;
};


#endif //MESSAGE_PASS_COMPARISON_CONNECTION_H
