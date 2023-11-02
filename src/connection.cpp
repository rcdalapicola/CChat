#include "connection.h"

#include <iostream>

#include <unistd.h>     // close()
#include <arpa/inet.h>
#include <string.h>
#include <memory>


Connection::Connection(){
    socketConnection = socket(AF_INET, SOCK_STREAM, 0);
}

Connection::Connection(int socket_p) : socketConnection(socket_p) {
}

Connection::Connection(const Connection& socket_p) : 
                socketConnection(socket_p.socketConnection),
                onMessageReceivedCallback(socket_p.onMessageReceivedCallback),
                onTransmissionEndedCallback(socket_p.onTransmissionEndedCallback)
{

}

int Connection::openWriterConnection(const char *ip, int port) {
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

int Connection::sendGreeting() {
    Message greeting;
    if (mUserName.empty()) {
        std::cout << "Cannot send greetings before defining userName." << std::endl;
        return -1;
    }
    greeting.user(mUserName.c_str());
    greeting.content("");
    return send(socketConnection, greeting.serializeMessage(), MESSAGE_TOTAL_BUFFER_SIZE, 0);
};

int Connection::sendMessage(const Message& message) const {
    const auto content = message.getContent(); 
    if (content[0] == '\0') {
        std::cout << "Messages with empty content cannot be sent." << std::endl;
        return -1;
    }
    return send(socketConnection, message.serializeMessage(), MESSAGE_TOTAL_BUFFER_SIZE, 0);
}

int Connection::openListenerConnection(const char *ip, int port) {
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

int Connection::openListenerConnection(int port) {
    return openListenerConnection(nullptr, port); // Listen on all available interfaces
}

Connection* Connection::getIncomingConnection() {
    int clientSocket;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);

    clientSocket = accept(socketConnection, (struct sockaddr*)&clientAddress, &clientAddressLength);
    if (clientSocket == -1) {
        std::cout << "Error accepting client connection" << std::endl;
        close(socketConnection);
        return new Connection(-1);
    }

    return new Connection(clientSocket);
}

void Connection::closeConnection() {
    close(socketConnection);
}

const char* Connection::getUserName() const {
    return mUserName.c_str();
}

void Connection::userName(const char* userName) {
    mUserName = std::string(userName);
}

void Connection::processIncomingMessages(ConnectionList& socketList) {
    ssize_t bytesRead;
    char buffer[MESSAGE_TOTAL_BUFFER_SIZE];
    auto connection = socketConnection;

    while ((bytesRead = recv(connection, buffer, MESSAGE_TOTAL_BUFFER_SIZE, 0)) > 0) {
        buffer[bytesRead] = '\0';
        Message incomingMessage(buffer);
        auto contentLength = strlen(incomingMessage.getContent());
        if (contentLength == 0 && onGreetingReceivedCallback) {
            onGreetingReceivedCallback(incomingMessage, socketList, *this);
        }
        else if (contentLength != 0 && onMessageReceivedCallback) {
            onMessageReceivedCallback(incomingMessage, socketList, *this);
        }
    }

    if (onTransmissionEndedCallback) {
        onTransmissionEndedCallback(socketList, this);
    }
}

void Connection::onMessageReceived(const std::function<void(const Message&, ConnectionList&, Connection&)> callbackFunction) {
    onMessageReceivedCallback = callbackFunction;
}

void Connection::onGreetingReceived(const std::function<void(const Message&, ConnectionList&, Connection&)> callbackFunction) {
    onGreetingReceivedCallback = callbackFunction;
}

void Connection::onTransmissionEnded(std::function<void(ConnectionList&, Connection*)> callbackFunction) {
    onTransmissionEndedCallback = callbackFunction;
}
