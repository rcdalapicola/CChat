#include <iostream>
#include <sys/socket.h> // fsocket()
#include <netinet/in.h> // ?
#include <unistd.h>     // close()
#include "socket.h"
#include <functional>
#include <thread>
#include <vector>

void processIncomingMessages(const Socket* listenerSocket, const std::function<void(const char*)>& callbackFunction) {
    ssize_t bytesRead;
    static const int bufferSize = 512;
    char buffer[bufferSize];

    while ((bytesRead = recv(listenerSocket->socketConnection, buffer, bufferSize, 0)) > 0) {
        buffer[bytesRead] = '\0'; 
        callbackFunction(buffer);
    }
}

int main() {
    Socket sck;
    std::cout << "Server soc: " << sck.socketConnection << std::endl;
    if (sck.openListenerConnection(8080) == -1) {
        std::cout << "Error binding server socket" << std::endl;
        return 1;
    }
    std::cout << "Server connected." << std::endl;

    std::vector<std::thread> threadList;
    int count = 0;
    do {
        Socket* listenerSocket = new Socket(sck.getIncomingConnection());

        if (listenerSocket->socketConnection == -1) {
            break;
        }

        auto processMessage = [&listenerSocket] () {
            auto messageCallback = [] (const char* message) {
                const Message *decodedMessage = reinterpret_cast<const Message*>(message);
                std::cout << decodedMessage->user <<": " << decodedMessage->content << std::endl;
            };
            processIncomingMessages(listenerSocket, messageCallback);
        };

        threadList.emplace_back(processMessage);
        count++;
        std::cout << "Emplaced thread " << count << std::endl;
    } while(1);

    std::cout << "Connection closed from server side. " << std::endl;

    return 0;
}
