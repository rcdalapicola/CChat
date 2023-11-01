#include <netinet/in.h> // sockaddr_in
#include <functional>
#include <memory>

static const int userBufferSize = 64;
static const int contentBufferSize = 512 - userBufferSize;

typedef struct {
    char user[userBufferSize];
    char content[contentBufferSize];
} Message;

class Connection {
    public:
        Connection();
        Connection(int socket);
        Connection(const Connection& socket);

        int openListenerConnection(const char *ip, int port);
        int openListenerConnection(int port);

        int openWriterConnection(const char *ip, int port);

        Connection getIncomingConnection(); //return socket for incoming connection

        void processIncomingMessages(std::vector<Connection>& socketList);
        int sendMessage(const Message& message) const;

        void closeConnection();
        bool operator==(const Connection& comparedSocket) const;

        void onMessageReceived(const std::function<void(const char*, std::vector<Connection>&, Connection&)> callbackFunction);
        void onTransmissionEnded(std::function<void(std::vector<Connection>&, Connection&)> callbackFunction);

        int socketConnection;
    private:
        std::function<void(const char*, std::vector<Connection>&, Connection&)> onMessageReceivedCallback;
        std::function<void(std::vector<Connection>&, Connection&)> onTransmissionEndedCallback;
};
