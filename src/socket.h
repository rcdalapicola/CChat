#include <netinet/in.h> // sockaddr_in
#include <functional>

class Socket {
    public:
        Socket();
        Socket(int socket);
        Socket(const Socket& socket);
        ~Socket();

        int openListenerConnection(const char *ip, int port);
        int openListenerConnection(int port);

        Socket getIncomingConnection(); //return socket for incoming connection

        void getIncomingMessages(const std::function<void(const char*)>& callbackFunction);
        
        int socketConnection;
    private:
};