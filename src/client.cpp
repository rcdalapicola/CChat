#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>     // close()

#include "socket.h"


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

int main(int argc, char *argv[]) {

    char *user, *ip;
    int port;

    readInput(argc, argv, user, ip, port);

    std::cout << "Starting chat as user \"" << user << "\". Connecting to server " << ip << ":" << port << "..." << std::endl;

    Socket writer;

    writer.openWriterConnection(ip, port);

    Message chatMessage;
    strncpy(chatMessage.user, user, userBufferSize);
    do {
        std::cout << "Enter your message: ";
        std::cin.getline(chatMessage.content, contentBufferSize);
        chatMessage.user[userBufferSize - 1] = '\0';

        writer.sendMessage(chatMessage);
    } while (chatMessage.content[0] != '\0');

    std::cout << "Connection closed from client side." << std::endl;

    return 0;
}
