#ifndef SERVER_CONNECTOR_HPP
#define SERVER_CONNECTOR_HPP

#include <sys/select.h>

#include <string>
#include <vector>

class Connector {
public:
    Connector();
    Connector(int port, std::string host_name, int max_clients);

private:
    struct {
        int port;
        std::string host_name;
        int max_clients;
    } connection_info_;

    struct {
        int server_fd;
        std::vector<int> client_fds;
        fd_set master_fds_set;
        int max_fd;
    } fds_;

    void connect();
    void acceptClient();
    int getFirstFreeClient();
};

#endif