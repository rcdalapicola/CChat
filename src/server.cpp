#include "server.h"

#include "connection.h"
#include "message.h"

#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>     // close()
#include <vector>


/* -------------------------- Forward declare callback functions ------------------------------- */
void handleClientGreeting(const Message& message,
                          ConnectionList& socketList,
                          Connection* currentConnection);
void handleClientMessage(const Message& message, ConnectionList& socketList, Connection* socket);
void handleTransmissionEnd(ConnectionList& socketList, Connection* socket);

/* ----------------------------- Implement Server functions ------------------------------------ */
int Server::setup(int port) {
    if (listenerConnection.openListenerConnection(port) == -1) {
        std::cout << "Error binding server socket to port " << port << "." << std::endl;
        return 1;
    }
    std::cout << "Server connected to port " << port << "." << std::endl;
    return 0;
}

int Server::run() {
    while (true) {
        clientConnections.emplace_back(listenerConnection.getIncomingConnection());
        auto& clientConnection = clientConnections.back();

        clientConnection->onMessageReceived(handleClientMessage);
        clientConnection->onTransmissionEnded(handleTransmissionEnd);
        clientConnection->onGreetingReceived(handleClientGreeting);

        auto processMessage = [&clientConnection] (ConnectionList* clientConnections) {
            clientConnection->processIncomingMessages(*clientConnections);
        };

        std::thread clientThread(processMessage, &clientConnections);
        clientThread.detach();
    }

    listenerConnection.closeConnection();

    return 0;
}

/* ----------------------------- Implement callback functions ---------------------------------- */
void handleClientGreeting(const Message& message,
                          ConnectionList& socketList,
                          Connection* currentConnection) {
    const auto userName = currentConnection->getUserName();

    if (strlen(userName) == 0) {
        currentConnection->userName(message.getUser());
    } else {
        std::cout << "Error: Received greeting from already aknowledged client." << std::endl;
        return;
    } 

    Message welcomeMessage;
    welcomeMessage.user("Server");
    std::ostringstream welcomeString;
    welcomeString << "The user \"" << currentConnection->getUserName() << "\" joined the chat.";
    welcomeMessage.content(welcomeString.str().c_str());
    std::cout << welcomeString.str() << std::endl;
    for (const auto& connection: socketList) {
        if (connection.get() != currentConnection) {
            connection->sendMessage(welcomeMessage);
        }
    }
}

void handleClientMessage(const Message& message, ConnectionList& connectionList, Connection* currentConnection) {
    const auto userName = currentConnection->getUserName();

    if (strlen(userName) == 0) {
        currentConnection->userName(message.getUser());
    } else if (strcmp(userName, currentConnection->getUserName()) != 0) {
        std::cout << "Error: Message is coming from unexpected user." << std::endl;
        return;
    } 

    std::cout << currentConnection->getUserName() <<": " << message.getContent() << std::endl;
    for (const auto& connection: connectionList) {
        connection->sendMessage(message);
    }
}

void handleTransmissionEnd(ConnectionList& connectionList, Connection* currentConnection) {
    currentConnection->closeConnection();

    Message exitMessage;
    exitMessage.user("Server");
    std::ostringstream exitString;
    exitString << "The user \"" << currentConnection->getUserName() << "\" left the chat.";
    exitMessage.content(exitString.str().c_str());

    std::cout << exitString.str() << std::endl;

    ConnectionList::iterator it = connectionList.begin();
    while (it != connectionList.end()) {
        const auto chatMember = it->get();
        if (chatMember == currentConnection) {
            connectionList.erase(it);
            continue;
        }
        chatMember->sendMessage(exitMessage);
        std::advance(it, 1);
    }
}
