#include <cstdio>

#include "Peer.h"

int main(int argc, char* argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);

    int port = -1;

    if (argc < 2)
    {
        printf("Please specify the port\n");
        exit(EXIT_FAILURE);
    } else {
        port = atoi(argv[1]);
    }

    printf("Hello from p2p poort %d\n", port);

    Peer peer(port);

    peer.run();
}