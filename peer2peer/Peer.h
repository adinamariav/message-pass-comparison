//
// Created by adina on 1/6/24.
//

#ifndef MESSAGE_PASS_COMPARISON_PEER_H
#define MESSAGE_PASS_COMPARISON_PEER_H

#include "Connection.h"
#include <sys/epoll.h>
#include <iostream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

class Peer {
    Connection conn;
    int epollfd;

public:
    explicit Peer(int port);

    void sendMessage(const std::string& message, const std::string& destinationIpAddress, int destinationPort) const;

    void run();
};


#endif //MESSAGE_PASS_COMPARISON_PEER_H
