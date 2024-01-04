//
// Created by adina on 1/4/24.
//

#include "ClientsManager.h"

int ClientsManager::nrClients = 16;

ClientsManager::ClientsManager() {
    for (int i= 0 ; i< nrClients; i++) {
        this->clients.push_back(new Client());
    }
}

void ClientsManager::sendMessages(int number) {
    for (int i = 0; i < number; i++) {
        clients[0]->sendToClient(clients[1]->getId(), "da");
        printf("%s\n", clients[1]->receive().c_str());
    }
}
