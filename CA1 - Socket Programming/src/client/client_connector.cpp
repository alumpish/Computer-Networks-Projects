#include "client_connector.hpp"

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdexcept>

#include "net.hpp"

Connector::Connector(int port, const std::string& host_name) {
    sock_fd_ = net::connectTCPClient(host_name, port);
}

void Connector::sendMessage(const std::string& msg) {
    net::sendMessage(sock_fd_, msg);
}

std::string Connector::rcvMessage() {
    return net::rcvMessage(sock_fd_);
}

void Connector::closeConnection() {
    close(sock_fd_);
}
