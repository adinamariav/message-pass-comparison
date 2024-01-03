#include <cstdio>
#include "ConnectionManager.h"

int main()
{
    printf("Hello from server\n");

    ConnectionManager conn;

    conn.acceptConnections();
}