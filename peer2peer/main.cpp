#include <cstdio>

#include "Peer.h"

int main(int argc, char* argv[])
{
    printf("Hello from p2p\n");

    int port = -1;

    if (argc < 2)
    {
        printf("Please specify the port\n");
        exit(EXIT_FAILURE);
    } else {
        port = atoi(argv[1]);
    }

    Peer peer(port);

    peer.run();
}