#include "chatlib/client.h"

#include <cstring>


int readInput(int argc, char *argv[], char*& userName, char*& ip, int& port);

int main(int argc, char *argv[]) {
    char *userName;
    char *ip;
    int port;

    readInput(argc, argv, userName, ip, port);

    Client client;
    if (client.setup(userName, ip, port) != 0) {
        exit(-1);
    }
    auto runStatus = client.run();

    return runStatus;
}

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
