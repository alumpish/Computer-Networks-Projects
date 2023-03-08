#include "server_connector.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "exceptions.hpp"

Connector::Connector() {}

Connector::Connector(int port, std::string host_name, int max_clients)
    : connection_info_({port, host_name, max_clients}) {

    connect();
    fds_.client_fds = std::vector<int>(max_clients, -1);
    FD_ZERO(&fds_.master_fds_set);
    FD_SET(fds_.server_fd, &fds_.master_fds_set);
    FD_SET(STDIN_FILENO, &fds_.master_fds_set);
    fds_.max_fd = std::max(fds_.server_fd, STDIN_FILENO);
}

void Connector::connect() {
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(connection_info_.host_name.c_str());
    address.sin_port = htons(connection_info_.port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
        close(server_fd);
        throw InternalServerErr("Binding server address failed.");
    }

    listen(server_fd, connection_info_.max_clients);
    fds_.server_fd = server_fd;
}

void Connector::acceptClient() {
    int free_client_idx = getFirstFreeClient();
    if (free_client_idx == -1)
        throw InternalServerErr("Max number of clients exceeded.");

    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);

    client_fd = accept(fds_.server_fd, (struct sockaddr*)&client_address, (socklen_t*)&address_len);
    fds_.client_fds[free_client_idx] = client_fd;
    FD_SET(client_fd, &fds_.master_fds_set);
    fds_.max_fd = std::max(fds_.max_fd, client_fd);
}

int Connector::getFirstFreeClient() {
    for (int i = 0; i < connection_info_.max_clients; ++i)
        if (fds_.client_fds[i] != -1)
            return i;
    return -1;
}
