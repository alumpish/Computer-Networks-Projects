#ifndef CLIENT_CONNECTOR_HPP
#define CLIENT_CONNECTOR_HPP

#include <string>

class Connector {
public:
    Connector(int port, const std::string& host_name);

    void sendMessage(const std::string& msg);
    std::string rcvMessage();
    void closeConnection();

private:
    int sock_fd_;
};

#endif