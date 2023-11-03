#ifndef CONNECTION_H
#define CONNECTION_H

#include "message.h"

#include <functional>
#include <memory>
#include <string>
#include <vector>


class Connection;
typedef std::vector<std::unique_ptr<Connection>> ConnectionList;

class Connection {
public:
    /**
     * @brief Create a new socket for a new connection
     * 
     */
    Connection();
    /**
     * @brief Creates a Connection object using the input socket for the connection.
     * 
     * @param socket 
     */
    Connection(int socket);
    /**
     * @brief Copies a Connection object
     * 
     * @param connection 
     */
    Connection(const Connection& connection);

    /**
     * @brief Creates a socket to listen for the given IP on the given port
     * 
     * @param ip 
     * @param port 
     * @return int 0 if succeeded, 1 if failed. 
     */
    int openListenerConnection(const char *ip, int port);
    /**
     * @brief Creates a listener socket on the given port for any IP.
     * 
     * @param port 
     * @return int 0 if succeeded, 1 if failed. 
     */
    int openListenerConnection(int port);

    /**
     * @brief Creates a socket to send messages to the given ip and port.
     * 
     * @param ip 
     * @param port 
     * @return int 0 if succeeded, 1 if failed. 
     */
    int openWriterConnection(const char *ip, int port);

    /**
     * @brief Accepts a new incoming connection
     * 
     * @return Connection* Connection object of the new incoming connection
     */
    Connection* getIncomingConnection();

    /**
     * @brief This function will keep listening to the incoming messages, and calling the
     *        proper callback functions as the messages arrive. It's recommended to run this
     *        function within a thread, since it will freeze the code while the connection
     *        is not terminated.
     * 
     * @param connectionList list of all openned connections.
     */
    void processIncomingMessages(ConnectionList* connectionList);

    /**
     * @brief Sends a greeting message. This should be sent before any other message, and only once.
     * 
     * @return int If the sending fails, this will return -1.
     */
    int sendGreeting();
    /**
     * @brief Sends a message to the connected peer.
     * 
     * @param message Message object to be sent
     * @return int If the sending fails, this will return -1.
     */
    int sendMessage(const Message& message) const;

    /**
     * @brief Closes connection with peer.
     * 
     */
    void closeConnection();

    /**
     * @brief Callback function that will be called if a regular message is received.
     * 
     * @param callbackFunction 
     */
    void onMessageReceived(const std::function<void(const Message&, ConnectionList*, Connection*)> callbackFunction);
    /**
     * @brief Callback function that will be called if a greeting message is received.
     * 
     * @param callbackFunction 
     */
    void onGreetingReceived(const std::function<void(const Message&, ConnectionList*, Connection*)> callbackFunction);
    /**
     * @brief Callback function that will be called when the connection with peer ends.
     * 
     * @param callbackFunction 
     */
    void onTransmissionEnded(std::function<void(ConnectionList*, Connection*)> callbackFunction);

    /**
     * @brief Gets the UserName of this connection.
     * 
     * @return const char* connection UserName.
     */
    const char* getUserName() const;
    /**
     * @brief Sets the UserName of this connection.
     * 
     * @param userName connection UserName.
     */
    void userName(const char* userName);

private:
    const int socketConnection;

    std::function<void(const Message&, ConnectionList*, Connection*)> onMessageReceivedCallback;
    std::function<void(const Message&, ConnectionList*, Connection*)> onGreetingReceivedCallback;
    std::function<void(ConnectionList*, Connection*)> onTransmissionEndedCallback;
    std::string mUserName;
};

#endif //CONNECTION_H
