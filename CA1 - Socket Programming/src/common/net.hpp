#ifndef TCP_HPP
#define TCP_HPP

#include <string>

namespace net {

int connectTCPServer(const std::string& host_name, int port, int max_clients);

int connectTCPClient(const std::string& host_name, int port);

std::string rcvMessage(int sock_fd);

void sendMessage(int sock_fd, std::string message);

int acceptClient(int server_fd);

} // namespace tcp

#endif