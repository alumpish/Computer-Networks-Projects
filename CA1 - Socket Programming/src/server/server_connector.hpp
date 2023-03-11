#ifndef SERVER_CONNECTOR_HPP
#define SERVER_CONNECTOR_HPP

#include <sys/select.h>

#include <string>
#include <vector>

class Connector {
public:

    struct Event {
        enum class EventType {
            stdin_cmd,
            incoming_client,
            client_req
        } type;
        int sock_fd;
    };

    Connector();
    Connector(int port, std::string host_name, int max_clients);

    std::string rcvMessage(int sock_fd);
    void sendMessage(int sock_fd, const std::string& msg);
    void acceptClient();

    Connector::Event pollForEvent();

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
    int getFirstFreeClient();
};

#endif