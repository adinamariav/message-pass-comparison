//
// Created by adina on 1/4/24.
//

#ifndef MESSAGE_PASS_COMPARISON_CLIENTSMANAGER_H
#define MESSAGE_PASS_COMPARISON_CLIENTSMANAGER_H

#include <vector>
#include "Client.h"

class ClientsManager {
    static int nrClients;
    std::vector<Client*> clients;

public:
    ClientsManager();

    void sendMessages(int number);
};


#endif //MESSAGE_PASS_COMPARISON_CLIENTSMANAGER_H
