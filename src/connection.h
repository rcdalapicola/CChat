#ifndef CONNECTION_H
#define CONNECTION_H

#include "message.h"

#include <functional>
#include <memory>
#include <string>


class Connection;
typedef std::vector<std::unique_ptr<Connection>> ConnectionList;

class Connection {
    public:
        Connection();
        Connection(int socket);
        Connection(const Connection& socket);

        int openListenerConnection(const char *ip, int port);
        int openListenerConnection(int port);

        int openWriterConnection(const char *ip, int port);

        Connection* getIncomingConnection(); //return socket for incoming connection

        void processIncomingMessages(ConnectionList& socketList);
        int sendGreeting();
        int sendMessage(const Message& message) const;

        void closeConnection();

        void onMessageReceived(const std::function<void(const Message&, ConnectionList&, Connection&)> callbackFunction);
        void onGreetingReceived(const std::function<void(const Message&, ConnectionList&, Connection&)> callbackFunction);
        void onTransmissionEnded(std::function<void(ConnectionList&, Connection*)> callbackFunction);

        const char* getUserName() const;
        void userName(const char* userName);

        int socketConnection;
    private:
        std::function<void(const Message&, ConnectionList&, Connection&)> onMessageReceivedCallback;
        std::function<void(const Message&, ConnectionList&, Connection&)> onGreetingReceivedCallback;
        std::function<void(ConnectionList&, Connection*)> onTransmissionEndedCallback;
        std::string mUserName;
};

#endif //CONNECTION_H
