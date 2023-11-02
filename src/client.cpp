#include "client.h"

#include "connection.h"

#include <iostream>
#include <thread>
#include <cstring>


void handleServerMessage(const Message& message, const ConnectionList& socketList, Connection& socket) {
    if (strcmp(socket.getUserName(), message.getUser()) != 0) {
        std::cout << message.getUser() <<": " << message.getContent() << std::endl;
    }
}

void handleConnectionTermination(ConnectionList& connectionList, Connection* currentConnection) {
    std::cout << "Connection with the server was interrupted." << std::endl;
}

int Client::setup(const char* user, const char* ip, int port) {
    if (connection.openWriterConnection(ip, port) != 0) {
        return 1;
    };
    connection.onMessageReceived(handleServerMessage);
    connection.onTransmissionEnded(handleConnectionTermination);
    connection.userName(user);

    return 0;
}

int Client::run() {
    auto listenToServer = [] (Connection* connection) {  
        std::vector<std::unique_ptr<Connection>> test;
        connection->processIncomingMessages(test);
    };

    std::thread t1(listenToServer, &connection);
    t1.detach();

    connection.sendGreeting();

    Message chatMessage;
    chatMessage.user(connection.getUserName());
    std::cout << "You are logged in as \"" << connection.getUserName() << "\". You can start chatting!" << std::endl;
    char buffer[MESSAGE_CONTENT_BUFFER_SIZE];
    while (1) {
        std::cin.getline(buffer, sizeof(buffer));

        if (buffer[0] == '\0')
        {
            break;
        }

        chatMessage.content(buffer);
        connection.sendMessage(chatMessage);
    };

    close(connection.socketConnection);

    return 0;
}
