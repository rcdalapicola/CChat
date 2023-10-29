#include <iostream>
#include <sys/socket.h> // fsocket()
#include <netinet/in.h> // ?
#include <unistd.h>     // close()
#include "socket.h"
#include <functional>

int main() {
    Socket sck;
    std::cout << "Server soc: " << sck.socketConnection << std::endl;
    if (sck.openListenerConnection(8080) == -1) {
        std::cout << "Error binding server socket" << std::endl;
        return 1;
    }
    std::cout << "Server connected." << std::endl;
        
    Socket tst(sck.getIncomingConnection());

    auto messageCallback = [] (const char* message) {
            std::cout << "Received message: \"" << message << "\"" << std::endl;
    };
    
    tst.getIncomingMessages(messageCallback);

    close(tst.socketConnection);

    std::cout << "Connection stablished and closed from server. " << std::endl;

    return 0;
}
