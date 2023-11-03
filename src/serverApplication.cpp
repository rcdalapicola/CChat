#include "chatlib/server.h"

#include <iostream>


int readInput(int argc, char *argv[], int& port);

int main(int argc, char *argv[]) {
    int port;
    if (readInput(argc, argv, port) == -1) {
        std::cout << "Invalid port as input. Using default value." << std::endl;
    }

    Server server;
    if (server.setup(port) != 0) {
        exit(-1);
    };

    auto runStatus = server.run();

    return runStatus;
}

int readInput(int argc, char *argv[], int& port) {
    static const int defaultPort = 8080;

    if (argc < 2) {
        port = defaultPort;
        return 0;
    }

    static const int portMaxSize = 5;
    char* portString = (char*)malloc(portMaxSize + 1);

    int i = 0;
    for (; i < portMaxSize  && argv[1][i] != '\0'; i++) {
        portString[i] = argv[1][i];
    }
    if (argv[1][i] != '\0') {
        port = defaultPort;
        return -1;
    }

    portString[i] = '\0';
    try {
        port = std::stoi(portString);
    } catch (const std::invalid_argument&) {
        port = defaultPort;
        return -1;
    }  
    return 0;
}
