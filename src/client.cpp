#include "client.h"

#include "connection.h"

#include <iostream>
#include <thread>
#include <cstring>


/* -------------------------- Forward declare callback functions ------------------------------- */
void handleServerMessage(const Message& message, const ConnectionList* socketList, Connection* socket);
void handleConnectionTermination(ConnectionList* connectionList, Connection* currentConnection);

/* ----------------------------- Implement Client functions ------------------------------------ */
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
        connection->processIncomingMessages(nullptr);
    };

    std::thread listenerThread(listenToServer, &connection);
    listenerThread.detach();

    connection.sendGreeting();

    Message chatMessage;
    chatMessage.user(connection.getUserName());
    std::cout << "You are logged in as \"" << connection.getUserName() \
              <<  "\". You can start chatting!" << std::endl;

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

    connection.closeConnection();

    return 0;
}

/* ----------------------------- Implement callback functions ---------------------------------- */
void handleServerMessage(const Message& message,
                         const ConnectionList* socketList,
                         Connection* currentConnection) {
    if (strcmp(currentConnection->getUserName(), message.getUser()) != 0) {
        std::cout << message.getUser() <<": " << message.getContent() << std::endl;
    }
}

void handleConnectionTermination(ConnectionList* connectionList, Connection* currentConnection) {
    std::cout << "Connection with the server was interrupted." << std::endl;
}
