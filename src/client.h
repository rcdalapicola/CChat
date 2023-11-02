#include "connection.h"

class Client {
    public:
        int setup(const char* user, const char* ip, int port);
        int run();
    private:
        Connection connection;
};