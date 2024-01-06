//
// Created by adina on 1/3/24.
//

#ifndef MESSAGE_PASS_COMPARISON_IEPOLLWRAPPER_H
#define MESSAGE_PASS_COMPARISON_IEPOLLWRAPPER_H

#include <string>
#include <vector>

class IEpollWrapper {
public:
    [[nodiscard]] virtual int getId() const = 0;

    virtual void setId(int id) = 0;

    [[nodiscard]] virtual int getDescriptor() const = 0;

    virtual void sendMessage(std::string message) = 0;

    virtual std::string receiveMessage() = 0;

    virtual std::vector<std::string> receiveMessages() = 0;

    virtual ~IEpollWrapper() = default;
};

#endif //MESSAGE_PASS_COMPARISON_IEPOLLWRAPPER_H
