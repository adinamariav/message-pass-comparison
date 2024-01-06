//
// Created by adina on 1/4/24.
//

#ifndef MESSAGE_PASS_COMPARISON_CLIENT_H
#define MESSAGE_PASS_COMPARISON_CLIENT_H

#include "Connection.h"

class Client {
    Connection conn;
public:
    [[nodiscard]] const Connection &getConn() const;

private:
    int id = -1;

public:
    explicit Client(int id);

    [[nodiscard]] int getId() const;

    void sendToClient(int id, const std::string& content);
    std::vector<std::string> receive();
};


#endif //MESSAGE_PASS_COMPARISON_CLIENT_H
