#include <iostream>
#include <sys/socket.h> // fsocket()
#include <netinet/in.h> // ?
#include <unistd.h>     // close()
#include "connection.h"
#include <functional>
#include <thread>
#include <vector>
#include <memory>


void handleClientMessage(const char* message, ConnectionList& socketList, Connection& socket) {
    const Message *decodedMessage = reinterpret_cast<const Message*>(message);
    std::cout << decodedMessage->user <<": " << decodedMessage->content << std::endl;
    for (const auto& socket: socketList) {
        socket->sendMessage(*decodedMessage);
    }
}

void handleTransmissionEnd(ConnectionList& socketList, Connection& socket) {
    socket.closeConnection();
    // auto it = std::find(socketList.begin(), socketList.end(), socket);
    // if (it != socketList.end()) {
    //     socketList.erase(it);
    // }

    std::cout << "Client disconnected." << std::endl;
}

int main() {
    Connection sck;

    if (sck.openListenerConnection(8081) == -1) {
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
