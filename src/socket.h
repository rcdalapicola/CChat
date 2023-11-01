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
        ~Socket();

        int openListenerConnection(const char *ip, int port);
        int openListenerConnection(int port);

        int openWriterConnection(const char *ip, int port);

        Socket getIncomingConnection(); //return socket for incoming connection

        void processIncomingMessages(const std::function<void(const char*, const Socket&)>& callbackFunction);
        int sendMessage(const Message& message) const;

        int socketConnection;
    private:
};
