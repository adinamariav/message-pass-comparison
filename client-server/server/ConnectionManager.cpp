//
// Created by adina on 1/3/24.
//

#include "ConnectionManager.h"
#include "ServerConnection.h"

int ConnectionManager::id_seed = 0;

ConnectionManager::ConnectionManager() {
    this->epollFd = epoll_create1(0);

    IConnection* serverConnection = new ServerConnection();
    struct epoll_event serverEvent;

    serverEvent.events = EPOLLIN;
    serverEvent.data.ptr = serverConnection;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverConnection->getSocket(), &serverEvent) == -1) {
        perror("Failed to add file descriptor to epoll");
        exit(EXIT_FAILURE);
    }

}


[[noreturn]] void ConnectionManager::acceptConnections() {
    while(true) {
        int numEvents = epoll_wait(epollFd, events, 16, -1);
        for (int i = 0; i < numEvents; i++) {
            auto* wrapper = static_cast<IConnection*>(events[i].data.ptr);

            if (wrapper->getId() == 0)
                handleServer(wrapper);
            else if (events[i].events & EPOLLIN) {
                handleClient(wrapper);
            }
        }
    }
}

void ConnectionManager::handleServer(IConnection *connection) {
    struct sockaddr_in clientAddr;
    socklen_t socklen = sizeof(clientAddr);
    char buf[1024];

    int newSock = accept(connection->getSocket(), (struct sockaddr*)&clientAddr, &socklen);

    inet_ntop(AF_INET, (char *)&(clientAddr.sin_addr),
              buf, sizeof(clientAddr));
    printf("[+] connected with %s:%d\n", buf,
           ntohs(clientAddr.sin_port));

    IConnection *clientConnection = new Connection(newSock, getNextId());
    this->connections.push_back(clientConnection);

    std::string clientBuf = "Hello! Your id is " + std::to_string(clientConnection->getId()) + "\n";

    clientConnection->sendMessage(clientBuf);

    struct epoll_event clientEvent;

    clientEvent.events = EPOLLIN;
    clientEvent.data.ptr = clientConnection;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientConnection->getSocket(), &clientEvent) == -1) {
        perror("Failed to add file descriptor to epoll");
        exit(EXIT_FAILURE);
    }
}

int ConnectionManager::getNextId() {
    return ++id_seed;
}

void ConnectionManager::handleClient(IConnection *connection) {
    std::string received = connection->receiveMessage();

    typedef std::vector<std::string> Tokens;
    Tokens tokens;
    std::string content;

    boost::split(tokens, received, boost::is_any_of(" "));

    if (tokens[0] == "SEND") {
        int sendId = stoi(tokens[1]);
        auto iter = tokens.begin();
        std::advance(iter, 2);

        for (; iter != tokens.end(); iter++)
            content += *iter + " ";

        std::string mes = "[*] id " + std::to_string(connection->getId()) + " wants to send " + std::to_string(sendId) + " " + content;
        printf("%s", mes.c_str());

        for (auto c : this->connections) {
            if (c->getId() == sendId) {
                std::string toSend = "[" + std::to_string(connection->getId()) + "]: " + content;
                c->sendMessage(toSend);
            }
        }
    } else {
        connection->sendMessage(received);
    }
}
