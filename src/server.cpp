#include <iostream>
#include <sys/socket.h> // fsocket()
#include <netinet/in.h> // ?
#include <unistd.h>     // close()
#include "socket.h"


int main() {
    Socket sck;
    std::cout << "Server soc: " << sck.socketConnection << std::endl;
    if (sck.openListenerConnection(8080) == -1) {
        std::cout << "Error binding server socket" << std::endl;
        return 1;
    }
    std::cout << "Server connected." << std::endl;

    Socket client = sck.getIncomingConnection();

    static const int bufferSize = 256;
    char buffer[bufferSize];

    client.getIncomingMessage(buffer, bufferSize);

    std::cout << "Message recieved form client: " << buffer << std::endl;

    std::cout << "Connection stablished and closed from server. " << std::endl;

    return 0;
}