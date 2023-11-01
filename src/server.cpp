#include <iostream>
#include <sys/socket.h> // fsocket()
#include <netinet/in.h> // ?
#include <unistd.h>     // close()
#include "socket.h"
#include <functional>
#include <thread>
#include <vector>


void handleClientMessage(const char* message, std::vector<Socket>& socketList, Socket& socket) {
    const Message *decodedMessage = reinterpret_cast<const Message*>(message);
    std::cout << decodedMessage->user <<": " << decodedMessage->content << std::endl;
    for (const auto& socket: socketList) {
        socket.sendMessage(*decodedMessage);
    }
}

void handleTransmissionEnd(std::vector<Socket>& socketList, Socket& socket) {
    socket.closeConnection();
    auto it = std::find(socketList.begin(), socketList.end(), socket);
    if (it != socketList.end()) {
        socketList.erase(it);
    }

    std::cout << "Client disconnected." << std::endl;
}

int main() {
    Socket sck;

    if (sck.openListenerConnection(8081) == -1) {
        std::cout << "Error binding server socket" << std::endl;
        return 1;
    }
    std::cout << "Server connected." << std::endl;

    std::vector<Socket> socketList;
    while (true) {
        socketList.emplace_back(sck.getIncomingConnection());
        auto& listenerSocket = socketList.back();

        std::cout << "Client connected!" << std::endl;

        listenerSocket.onMessageReceived(handleClientMessage);
        listenerSocket.onTransmissionEnded(handleTransmissionEnd);

        auto processMessage = [&listenerSocket, &socketList] () {
            Socket tst(listenerSocket);
            tst.processIncomingMessages(socketList);
        };

        std::thread clientThread(processMessage);
        clientThread.detach();
    }

    close(sck.socketConnection);
    return 0;
}
