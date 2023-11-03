#include "connection.h"

class Client {
public:
    /**
     * @brief Set up a client with the given UserName, IP, and port.
     * 
     * @param user 
     * @param ip 
     * @param port 
     * @return int 0 if successful, 1 if failed.
     */
    int setup(const char* user, const char* ip, int port);
    /**
     * @brief Runs the client. This will freeze the code until the client is finished.
     * 
     * @return int 0 if successful.
     */
    int run();
private:
    Connection connection;
};
