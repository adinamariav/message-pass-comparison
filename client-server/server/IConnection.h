//
// Created by adina on 1/3/24.
//

#ifndef MESSAGE_PASS_COMPARISON_ICONNECTION_H
#define MESSAGE_PASS_COMPARISON_ICONNECTION_H

#include <string>

class IConnection {
public:
    [[nodiscard]] virtual int getId() const = 0;
    [[nodiscard]] virtual int getSocket() const = 0;
    virtual void initialize() = 0;
    virtual void handle() = 0;
    virtual void sendMessage(std::string message) = 0;
    virtual std::string receiveMessage() = 0;

    virtual ~IConnection() = default;
};

#endif //MESSAGE_PASS_COMPARISON_ICONNECTION_H
