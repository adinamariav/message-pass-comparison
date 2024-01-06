//
// Created by adina on 1/6/24.
//

#include "Peer.h"

Peer::Peer(int port) : conn(port) {

}

void Peer::sendMessage(const std::string& message, const std::string& destinationIpAddress, int destinationPort) const {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Socket creation failed");
        return;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(destinationPort);
    inet_pton(AF_INET, destinationIpAddress.c_str(), &serverAddress.sin_addr);

    if (connect(sock, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Connection failed");
        close(sock);
        return;
    }

    auto messageSize = static_cast<std::uint32_t>(message.size());

    if (send(sock, &messageSize, sizeof(messageSize), 0) == -1) {
        perror("Error sending message size");
        exit(EXIT_FAILURE);
    }

    const char* data = message.c_str();
    size_t remaining = message.size();
    ssize_t sent;

    while (remaining > 0) {
        sent = send(sock, data, remaining, 0);

        if (sent == -1) {
            perror("Error sending message");
            exit(EXIT_FAILURE);
        }

        data += sent;
        remaining -= static_cast<size_t>(sent);
    }

    close(sock);
}


void Peer::run() {
    epollfd = epoll_create1(0);

    struct epoll_event stdinEvent;
    stdinEvent.events = EPOLLIN;
    stdinEvent.data.fd = 0;

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, 0, &stdinEvent) == -1) {
        perror("Failed to add file descriptor to epoll\n");
        exit(EXIT_FAILURE);
    }

    struct epoll_event peerEvent;
    peerEvent.events = EPOLLIN;
    peerEvent.data.fd = this->conn.getDescriptor();

    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, this->conn.getDescriptor(), &peerEvent) == -1) {
        perror("Failed to add file descriptor to epoll\n");
        exit(EXIT_FAILURE);
    }

    std::vector<struct epoll_event> events(2);
    std::string received;
    int sendPort;
    typedef std::vector<std::string> Tokens;
    Tokens tokens;
    std::string content;

    while (true) {
        int numEvents = epoll_wait(epollfd, events.data(), 2, -1);

        for (int i = 0; i < numEvents; i++) {
            if (events[i].data.fd == 0) {
                getline(std::cin, received);

                if (received.empty())
                    continue;

                boost::split(tokens, received, boost::is_any_of(" "));
                try {
                    sendPort = stoi(tokens[0]);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid argument: " << tokens[0] << std::endl;
                    continue;
                }

                auto iter = tokens.begin();
                std::advance(iter, 1);
                for (; iter != tokens.end(); iter++)
                    content += *iter + " ";
                this->sendMessage(content, "127.0.0.1", sendPort);
            } else {
                struct sockaddr_in clientAddr;
                socklen_t socklen = sizeof(clientAddr);
                char buf[1024];

                int newSock = accept(this->conn.getDescriptor(),(struct sockaddr*)&clientAddr, &socklen);

                auto receivedMessages =  this->conn.receiveMessages(newSock);

                if (receivedMessages.empty())
                    break;

                inet_ntop(AF_INET, (char *)&(clientAddr.sin_addr),
                          buf, sizeof(clientAddr));
                struct sockaddr_in peerAddr;
                socklen_t peerAddrLen = sizeof(peerAddr);
                getpeername(newSock, (struct sockaddr*)&peerAddr, &peerAddrLen);

                for (const auto& mes : receivedMessages) {
                    printf("%s:%d: ", buf, ntohs(peerAddr.sin_port));
                    std::cout << mes << "\n";
                }

                close(newSock);
            }

            received = "";
            tokens.clear();
            content = "";
        }
    }
}
