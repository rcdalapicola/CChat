#include <netinet/in.h> // sockaddr_in
#include <functional>

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

        void getIncomingMessages(const std::function<void(const char*)>& callbackFunction);
        int sendMessage(const Message& message);

        int socketConnection;
    private:
};
