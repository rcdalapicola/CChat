#include <netinet/in.h> // sockaddr_in
#include <functional>
#include <memory>

static const int userBufferSize = 64;
static const int contentBufferSize = 512 - userBufferSize;

typedef struct {
    char user[userBufferSize];
    char content[contentBufferSize];
} Message;

class Socket {
    public:
        Socket();
        Socket(int socket);
        Socket(const Socket& socket);

        int openListenerConnection(const char *ip, int port);
        int openListenerConnection(int port);

        int openWriterConnection(const char *ip, int port);

        Socket getIncomingConnection(); //return socket for incoming connection

        void processIncomingMessages(std::vector<Socket>& socketList);
        int sendMessage(const Message& message) const;

        void closeConnection();
        bool operator==(const Socket& comparedSocket) const;

        void onMessageReceived(const std::function<void(const char*, std::vector<Socket>&, Socket&)> callbackFunction);
        void onTransmissionEnded(std::function<void(std::vector<Socket>&, Socket&)> callbackFunction);

        int socketConnection;
    private:
        std::function<void(const char*, std::vector<Socket>&, Socket&)> onMessageReceivedCallback;
        std::function<void(std::vector<Socket>&, Socket&)> onTransmissionEndedCallback;
};
