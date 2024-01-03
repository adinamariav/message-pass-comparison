//
// Created by adina on 1/3/24.
//

#include "ConnectionManager.h"
#include "ServerConnection.h"
#include "Signal.h"

int ConnectionManager::id_seed = 0;

ConnectionManager::ConnectionManager() {
    this->epollFd = epoll_create1(0);

    IEpollWrapper* serverConnection = new ServerConnection();
    struct epoll_event serverEvent;

    serverEvent.events = EPOLLIN;
    serverEvent.data.ptr = serverConnection;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverConnection->getDescriptor(), &serverEvent) == -1) {
        perror("Failed to add file descriptor to epoll");
        exit(EXIT_FAILURE);
    }

    IEpollWrapper* signalHandler = new Signal();
    struct epoll_event signalEvent;

    signalEvent.events = EPOLLIN;
    signalEvent.data.ptr = signalHandler;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, signalHandler->getDescriptor(), &signalEvent) == -1) {
        perror("Failed to add signalfd signal handler");
        exit(EXIT_FAILURE);
    }
}


[[noreturn]] void ConnectionManager::acceptConnections() {
    while(true) {
        int numEvents = epoll_wait(epollFd, events, 16, -1);
        for (int i = 0; i < numEvents; i++) {
            auto* wrapper = static_cast<IEpollWrapper*>(events[i].data.ptr);

            if (wrapper->getId() == 0)
                handleServer(wrapper);
            else if (wrapper->getId() == -1)
                handleSignal(wrapper);
            else if (events[i].events & EPOLLIN)
                handleClient(wrapper);
        }
    }
}

void ConnectionManager::handleServer(IEpollWrapper *connection) {
    struct sockaddr_in clientAddr;
    socklen_t socklen = sizeof(clientAddr);
    char buf[1024];

    int newSock = accept(connection->getDescriptor(), (struct sockaddr*)&clientAddr, &socklen);

    inet_ntop(AF_INET, (char *)&(clientAddr.sin_addr),
              buf, sizeof(clientAddr));
    printf("[+] connected with %s:%d\n", buf,
           ntohs(clientAddr.sin_port));

    IEpollWrapper *clientConnection = new Connection(newSock, getNextId());
    this->connections.push_back(clientConnection);

    std::string clientBuf = "Hello! Your id is " + std::to_string(clientConnection->getId()) + "\n";

    clientConnection->sendMessage(clientBuf);

    struct epoll_event clientEvent;

    clientEvent.events = EPOLLIN;
    clientEvent.data.ptr = clientConnection;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientConnection->getDescriptor(), &clientEvent) == -1) {
        perror("Failed to add file descriptor to epoll");
        exit(EXIT_FAILURE);
    }
}

int ConnectionManager::getNextId() {
    return ++id_seed;
}

void ConnectionManager::handleClient(IEpollWrapper *connection) {
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

ConnectionManager::~ConnectionManager() {
    for (auto client : this->connections)
        delete client;

    this->connections.clear();
}

void ConnectionManager::handleSignal(IEpollWrapper *connection) {
    printf("Received signal!!!!!!");
    for (auto client : this->connections)
        delete client;

    this->connections.clear();
    exit(EXIT_SUCCESS);
}
