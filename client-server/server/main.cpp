#include <cstdio>
#include "ConnectionManager.h"

int main()
{
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("Hello from server\n");

    ConnectionManager conn;

    conn.acceptConnections();
}