#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>     // close()

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1) {
        std::cout << "Error creating client socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080); 
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cout << "Error connecting to the server" << std::endl;
        close(clientSocket);
        return 1;
    }

    char buffer[256];
    do {
        std::cout << "Enter your message: ";
        std::cin.getline(buffer, 256);

        send(clientSocket, buffer, strlen(buffer), 0);
    } while (buffer[0] != '\0');
    std::cout << "Connection stablished and closed from client side." << std::endl;

    close(clientSocket);

    return 0;
}
