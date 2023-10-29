#include <netinet/in.h> // sockaddr_in


class Socket {
    public:
        Socket();
        Socket(int socket);
        ~Socket();

        int openListenerConnection(const char *ip, int port);
        int openListenerConnection(int port);

        Socket getIncomingConnection(); //return socket for incoming connection

        void getIncomingMessage(char* buffer, int bufferSize);
        
        int socketConnection;
    private:
};