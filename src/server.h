#include "connection.h"

class Server {
    public:
        int setup(int port);
        int run();
    private:
        Connection listenerConnection;
        ConnectionList clientConnections;
};