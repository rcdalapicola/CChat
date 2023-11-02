#include <iostream>
#include <sys/socket.h> // fsocket()
#include <netinet/in.h> // ?
#include <unistd.h>     // close()
#include "connection.h"
#include "message.h"
#include <functional>
#include <thread>
#include <vector>
#include <memory>


void handleClientMessage(const Message& message, ConnectionList& socketList, Connection& socket) {
    std::cout << message.getUser() <<": " << message.getContent() << std::endl;
    for (const auto& socket: socketList) {
        socket->sendMessage(message);
    }
}

void handleTransmissionEnd(ConnectionList& socketList, Connection* socket) {
    socket->closeConnection();

    ConnectionList::iterator it = socketList.begin();
    while (it->get() != socket) {
        if (it == socketList.end()) {
            std::cout << "Unexpected error when deleting connection." << std::endl;
            return;
        }
        std::advance(it, 1);
    }
    socketList.erase(it);

    std::cout << "Client disconnected." << std::endl;
}

int main() {
    Connection sck;

    if (sck.openListenerConnection(8080) == -1) {
        std::cout << "Error binding server socket" << std::endl;
        return 1;
    }
    std::cout << "Server connected." << std::endl;

    ConnectionList socketList;
    while (true) {
        socketList.emplace_back(sck.getIncomingConnection());
        auto& listenerSocket = socketList.back();

        std::cout << "Client connected!" << std::endl;

        listenerSocket->onMessageReceived(handleClientMessage);
        listenerSocket->onTransmissionEnded(handleTransmissionEnd);

        auto processMessage = [&listenerSocket, &socketList] () {
            listenerSocket->processIncomingMessages(socketList);
        };

        std::thread clientThread(processMessage);
        clientThread.detach();
    }

    close(sck.socketConnection);
    return 0;
}
