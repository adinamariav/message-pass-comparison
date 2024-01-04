#include <cstdio>
#include <sys/epoll.h>
#include <vector>
#include <iostream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include "Client.h"

int main(int argc, char* argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("Hello from client!\n");
    int epollFd = epoll_create1(0);

    struct epoll_event stdinEvent;
    stdinEvent.events = EPOLLIN;
    stdinEvent.data.fd = 0;

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, 0, &stdinEvent) == -1) {
        perror("Failed to add file descriptor to epoll\n");
        exit(EXIT_FAILURE);
    }

    Client client;
    std::cout << "ID " << std::to_string(client.getId()) << "\n";

    struct epoll_event clientEvent;
    clientEvent.events = EPOLLIN;
    clientEvent.data.fd = client.getConn().getClientSocket();

    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, client.getConn().getClientSocket(), &clientEvent) == -1) {
        perror("Failed to add file descriptor to epoll\n");
        exit(EXIT_FAILURE);
    }

    std::vector<struct epoll_event> events(2);
    std::string received;
    int sendID;
    typedef std::vector<std::string> Tokens;
    Tokens tokens;
    std::string content;

    while (true) {
        int numEvents = epoll_wait(epollFd, events.data(), 2, -1);

        for (int i = 0; i < numEvents; i++) {
            if (events[i].data.fd == 0) {
                getline(std::cin, received);

                boost::split(tokens, received, boost::is_any_of(" "));
                sendID = stoi(tokens[0]);
                auto iter = tokens.begin();
                std::advance(iter, 1);
                for (; iter != tokens.end(); iter++)
                    content += *iter + " ";
                client.sendToClient(sendID, content);
            } else {
                received = client.receive();

                if (received.empty())
                    break;

                std::cout << "Received " + received << "\n";
            }
        }
    }
}