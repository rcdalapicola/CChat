#include "client.h"

#include "connection.h"

#include <cstring>
#include <iostream>
#include <thread>


/* -------------------------- Forward declare callback functions ------------------------------- */
void handleServerMessage(const Message& message, const ConnectionList* connectionList, Connection* currentConnection);
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
        // Callback functions don't use ConnectionList, so we can call
        // processIncomingMessages() with nullptr.
        connection->processIncomingMessages(nullptr);
    };

    std::thread listenerThread(listenToServer, &connection);
    listenerThread.detach();

    connection.sendGreeting();

    Message chatMessage;
    chatMessage.user(connection.getUserName());
    std::cout << "You are logged in as \"" << connection.getUserName() \
              <<  "\". You can start chatting!" << std::endl;

    // Main loop to get now messages from user and send them to server.
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
// Since none of these callbacks use the connectionList, we have pass this parameter as nullptr
// when calling processIncomingMessages().
void handleServerMessage(const Message& message,
                         const ConnectionList* connectionList,
                         Connection* currentConnection) {
    if (strcmp(currentConnection->getUserName(), message.getUser()) != 0) {
        std::cout << message.getUser() <<": " << message.getContent() << std::endl;
    }
}

void handleConnectionTermination(ConnectionList* connectionList, Connection* currentConnection) {
    std::cout << "Connection with the server was interrupted." << std::endl;
}
