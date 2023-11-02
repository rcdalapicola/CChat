#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>     // close()
#include <thread>

#include "connection.h"


void handleServerMessage(const char* message, const ConnectionList& socketList, Connection& socket) {
    const Message *decodedMessage = reinterpret_cast<const Message*>(message);
    std::cout << decodedMessage->user <<": " << decodedMessage->content << std::endl;
};

int readInput(int argc, char *argv[], char*& userName, char*& ip, int& port) {
    if (argc < 3) {
        return -1;
    }

    int len = strlen(argv[1]);

    userName = (char*)malloc(len + 1);
    strcpy(userName, argv[1]);

    static const int ipMaxSize = 15;
    static const int portMaxSize = 5;
    ip = (char*)malloc(ipMaxSize + 1);
    char* portString = (char*)malloc(portMaxSize + 1);

    fprintf(stderr, "%c", argv[2][0]);

    int i = 0;
    for (; i < ipMaxSize && argv[2][i] != ':' && argv[2][i] != '\0'; i++) {
        ip[i] = argv[2][i];
    }
    if (argv[2][i] != ':') {
        return -1;
    }
    ip[i] = '\0';
    i++; //skip the ':' character
    int j = 0;
    for (; j < portMaxSize  && argv[2][i+j] != '\0'; j++) {
        portString[j] = argv[2][i + j];
    }
    if (argv[2][i + j] != '\0') {
        return -1;
    }

    portString[j] = '\0';
    port = std::stoi(portString);
    return 0;
}

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>

void receiveMessages(int clientSocket) {
    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesRead] = '\0';
        std::cout << "Server says: " << buffer << std::endl;
    }
}

int main(int argc, char *argv[]) {    
    char *user, *ip;
    int port;

    readInput(argc, argv, user, ip, port);

    std::cout << "Starting chat as user \"" << user << "\". Connecting to server " << ip << ":" << port << "..." << std::endl;

    Connection writer;

    writer.openWriterConnection(ip, port);
    writer.onMessageReceived(handleServerMessage);

    auto listenToServer = [&writer] () {  
        std::vector<std::unique_ptr<Connection>> test;
        writer.processIncomingMessages(test);
    };

    std::thread t1(listenToServer);
    t1.detach();

    Message chatMessage;
    chatMessage.content[contentBufferSize - 1] = '\0';
    strncpy(chatMessage.user, user, userBufferSize);
    std::cout << "You can start chatting!" << std::endl;
    while (1) {
        std::cin.getline(chatMessage.content, contentBufferSize - 1);

        if (chatMessage.content[0] == '\0')
        {
            break;
        }

        writer.sendMessage(chatMessage);
    };

    close(writer.socketConnection);

    return 0;
}
