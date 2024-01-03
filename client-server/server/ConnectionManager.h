//
// Created by adina on 1/3/24.
//

#ifndef MESSAGE_PASS_COMPARISON_CONNECTIONMANAGER_H
#define MESSAGE_PASS_COMPARISON_CONNECTIONMANAGER_H

#include <sys/epoll.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <strings.h>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include "IConnection.h"

class ConnectionManager {
    struct epoll_event events[16];
    int epollFd;
    static int id_seed;

    std::vector<IConnection*> connections;

    static int getNextId();

public:
    ConnectionManager();

    void handleServer(IConnection* connection);
    void handleClient(IConnection* connection);

    [[noreturn]] void acceptConnections();
};


#endif //MESSAGE_PASS_COMPARISON_CONNECTIONMANAGER_H
