#include "socket.h"

#include <iostream>

#include <unistd.h>     // close()
#include <arpa/inet.h>
#include <string.h>
#include <memory>

const char* serializeMessage(const Message& message) {
    return reinterpret_cast<const char*>(&message);
}

bool Socket::operator==(const Socket& comparedSocket) const {
    return this->socketConnection == comparedSocket.socketConnection;
}

Socket::Socket(){
    socketConnection = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int socket_p) : socketConnection(socket_p) {
}

Socket::Socket(const Socket& socket_p) : socketConnection(socket_p.socketConnection),
                                         onMessageReceivedCallback(socket_p.onMessageReceivedCallback),
                                         onTransmissionEndedCallback(socket_p.onTransmissionEndedCallback)
{
    
}

int Socket::openWriterConnection(const char *ip, int port) {
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ip);

    if (connect(socketConnection, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cout << "Error connecting to the server" << std::endl;
        close(socketConnection);
        return 1;
    }

    return 0;
}

int Socket::sendMessage(const Message& message) const {
    return send(socketConnection, serializeMessage(message), 512, 0);
}

int Socket::openListenerConnection(const char *ip, int port) {
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = ip ? inet_addr(ip) : INADDR_ANY;

    if (bind(socketConnection, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cout << "Error binding server socket" << std::endl;
        close(socketConnection);
        return 1;
    }

    if (listen(socketConnection, 5) == -1) {
        std::cout << "Error listening for connections" << std::endl;
        close(socketConnection);
        return 1;
    }

    return 0;
}

int Socket::openListenerConnection(int port) {
    return openListenerConnection(nullptr, port); // Listen on all available interfaces
}

Socket Socket::getIncomingConnection() {
    int clientSocket;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    clientSocket = accept(socketConnection, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket == -1) {
        std::cout << "Error accepting client connection" << std::endl;
        close(socketConnection);
        return Socket(-1);
    }

    std::cout << "Client connected to server!" << std::endl;

    return Socket(clientSocket);
}

void Socket::closeConnection() {
    close(socketConnection);
}

void Socket::processIncomingMessages(std::vector<Socket>& socketList) {
    ssize_t bytesRead;
    static const int bufferSize = 512;
    char buffer[bufferSize];
    auto connection = socketConnection;

    while ((bytesRead = recv(connection, buffer, bufferSize, 0)) > 0) {
        buffer[bytesRead] = '\0';
        if (onMessageReceivedCallback) {
            onMessageReceivedCallback(buffer, socketList, *this);
        }
    }

    if (onTransmissionEndedCallback) {
        onTransmissionEndedCallback(socketList, *this);
    }

    std::cout << "Client ended" << std::endl;
}

void Socket::onMessageReceived(const std::function<void(const char*, std::vector<Socket>&, Socket&)> callbackFunction) {
    onMessageReceivedCallback = callbackFunction;
}

void Socket::onTransmissionEnded(std::function<void(std::vector<Socket>&, Socket&)> callbackFunction) {
    onTransmissionEndedCallback = callbackFunction;
}