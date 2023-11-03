#include "connection.h"

#include <arpa/inet.h>    // inet_addr()
#include <cstring>
#include <iostream>
#include <netinet/in.h>   // sockaddr_in
#include <unistd.h>       // close()


/* ------------------------------- Implement Constructors -------------------------------------- */
Connection::Connection() : 
                socketConnection(socket(AF_INET, SOCK_STREAM, 0))
{
}

Connection::Connection(int socket) : 
                socketConnection(socket)
{
}

Connection::Connection(const Connection& connection) : 
                socketConnection(connection.socketConnection),
                onMessageReceivedCallback(connection.onMessageReceivedCallback),
                onTransmissionEndedCallback(connection.onTransmissionEndedCallback)
{
}

/* ---------------------------- Functions for openning connections ----------------------------- */
int Connection::openListenerConnection(const char *ip, int port) {
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = ip ? inet_addr(ip) : INADDR_ANY;

    if (bind(socketConnection, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        close(socketConnection);
        return 1;
    }

    if (listen(socketConnection, 5) == -1) {
        close(socketConnection);
        return 2;
    }

    return 0;
}

int Connection::openListenerConnection(int port) {
    return openListenerConnection(nullptr, port); // Listen on all available interfaces
}

int Connection::openWriterConnection(const char *ip, int port) {
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ip);

    if (connect(socketConnection, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cout << "Error connecting to the server." << std::endl;
        close(socketConnection);
        return 1;
    }

    return 0;
}

/* ---------------------------- Functions for incoming connections ----------------------------- */
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


void Connection::processIncomingMessages(ConnectionList* connectionList) {
    ssize_t bytesRead;
    char buffer[MESSAGE_TOTAL_BUFFER_SIZE];

    while ((bytesRead = recv(socketConnection, buffer, MESSAGE_TOTAL_BUFFER_SIZE, 0)) > 0) {
        buffer[bytesRead] = '\0';
        Message incomingMessage(buffer);
        auto contentLength = strlen(incomingMessage.getContent());
        if (contentLength == 0 && onGreetingReceivedCallback) {
            onGreetingReceivedCallback(incomingMessage, connectionList, this);
        }
        else if (contentLength != 0 && onMessageReceivedCallback) {
            onMessageReceivedCallback(incomingMessage, connectionList, this);
        }
    }

    if (onTransmissionEndedCallback) {
        onTransmissionEndedCallback(connectionList, this);
    }
}

/* ------------------------------ Functions for sending messages ------------------------------- */
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

/* ------------------------------------ Callback functions ------------------------------------- */
void Connection::onMessageReceived(
        const std::function<void(const Message&, ConnectionList*, Connection*)> callbackFunction) {
    onMessageReceivedCallback = callbackFunction;
}

void Connection::onGreetingReceived(
        const std::function<void(const Message&, ConnectionList*, Connection*)> callbackFunction) {
    onGreetingReceivedCallback = callbackFunction;
}

void Connection::onTransmissionEnded(
        std::function<void(ConnectionList*, Connection*)> callbackFunction) {
    onTransmissionEndedCallback = callbackFunction;
}

/* --------------------------------------------------------------------------------------------- */
void Connection::closeConnection() {
    close(socketConnection);
}

const char* Connection::getUserName() const {
    return mUserName.c_str();
}

void Connection::userName(const char* userName) {
    mUserName = std::string(userName);
}
