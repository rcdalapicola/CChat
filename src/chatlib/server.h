#ifndef SERVER_H
#define SERVER_H

#include "connection.h"

class Server {
public:
    /**
     * @brief Set up a server on the given port.
     * 
     * @param port 
     * @return int 0 if successful, 1 if failed.
     */
    int setup(int port);
    /**
     * @brief Runs the server. This will freeze the code until the server is finished.
     * 
     * @return int 0 if successful.
     */
    int run();
private:
    Connection listenerConnection;
    ConnectionList clientConnections;
};

#endif //SERVER_H