#include <iostream>
#include <sys/socket.h> // fsocket()
#include <netinet/in.h> // ?
#include <unistd.h>     // close()
#include "socket.h"
#include <functional>
#include <thread>
#include <vector>


void handleClientMessage(const char* message, const Socket& socket) {
    const Message *decodedMessage = reinterpret_cast<const Message*>(message);
    std::cout << decodedMessage->user <<": " << decodedMessage->content << std::endl;
    socket.sendMessage(*decodedMessage);
}

int main() {
    Socket sck;

    if (sck.openListenerConnection(8080) == -1) {
        std::cout << "Error binding server socket" << std::endl;
        return 1;
    }
    std::cout << "Server connected." << std::endl;

    while (true) {
        auto listenerSocket = sck.getIncomingConnection();
        std::cout << "Client connected!" << std::endl;

        auto processMessage = [&listenerSocket] () {
            listenerSocket.processIncomingMessages(handleClientMessage);
        };

        std::thread clientThread(processMessage);
        clientThread.detach();
    }

    close(sck.socketConnection);
    return 0;
}
