#include "server_connector.hpp"

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <string>
#include <vector>

#include "exceptions.hpp"
#include "net.hpp"

Connector::Connector() {}

Connector::Connector(int port, std::string host_name, int max_clients)
    : connection_info_({port, host_name, max_clients}) {
    fds_.server_fd = net::connectTCPServer(host_name, port, max_clients);
    fds_.client_fds = std::vector<int>(max_clients, -1);
    FD_ZERO(&fds_.master_fds_set);
    FD_SET(fds_.server_fd, &fds_.master_fds_set);
    FD_SET(STDIN_FILENO, &fds_.master_fds_set);
    fds_.max_fd = std::max(fds_.server_fd, STDIN_FILENO);
}

std::string Connector::rcvMessage(int sock_fd) {
    return net::rcvMessage(sock_fd);
}

void Connector::sendMessage(int sock_fd, const std::string& msg) {
    return net::sendMessage(sock_fd, msg);
}

int Connector::acceptClient() {
    int free_client_idx = getFirstFreeClient();
    if (free_client_idx == -1)
        throw InternalServerErr("Max number of clients exceeded.");

    int client_fd = net::acceptClient(fds_.server_fd);
    fds_.client_fds[free_client_idx] = client_fd;
    FD_SET(client_fd, &fds_.master_fds_set);
    fds_.max_fd = std::max(fds_.max_fd, client_fd);
    return client_fd;
}

void Connector::removeConnection(int sock_fd) {
    for (auto& client : fds_.client_fds)
        if (client == sock_fd)
            client = -1;

    if (sock_fd == fds_.max_fd)
        fds_.max_fd = *std::max_element(fds_.client_fds.begin(), fds_.client_fds.end());

    FD_CLR(sock_fd, &fds_.master_fds_set);

    close(sock_fd);
}

Connector::Event Connector::pollForEvent() {
    fd_set working_set = fds_.master_fds_set;
    select(fds_.max_fd + 1, &working_set, nullptr, nullptr, nullptr);
    if (FD_ISSET(STDIN_FILENO, &working_set))
        return {Connector::Event::EventType::stdin_cmd, STDIN_FILENO};
    if (FD_ISSET(fds_.server_fd, &working_set))
        return {Connector::Event::EventType::incoming_client, fds_.server_fd};
    for (const auto& cur_client_fd : fds_.client_fds)
        if (FD_ISSET(cur_client_fd, &working_set))
            return {Connector::Event::EventType::client_req, cur_client_fd};
}

int Connector::getFirstFreeClient() {
    for (int i = 0; i < connection_info_.max_clients; ++i)
        if (fds_.client_fds[i] == -1)
            return i;
    return -1;
}
