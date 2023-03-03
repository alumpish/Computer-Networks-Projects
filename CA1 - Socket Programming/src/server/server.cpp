#include "server.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cmath>
#include <fstream>

#include "exceptions.hpp"
#include "json.hpp"

using json = nlohmann::json;

Server::Server(const std::string& config_file) {
    std::ifstream f(config_file);
    json data = json::parse(f);
    port_ = data["port"];
    host_name_ = data["hostName"];
    max_clients_ = data["maxClients"];

    connectTCP();
    fds_.client_fds = std::vector<int>(max_clients_, -1);
    FD_ZERO(&fds_.master_fds_set);
    FD_SET(fds_.server_fd, &fds_.master_fds_set);
    FD_SET(STDIN_FILENO, &fds_.master_fds_set);
    fds_.max_fd = std::max(fds_.server_fd, STDIN_FILENO);
}

void Server::run() {
}

void Server::addHandler(RequestHandler* handler) {
}

void Server::connectTCP() {
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(host_name_.c_str());
    address.sin_port = htons(port_);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
        close(server_fd);
        throw InternalServerErr("Binding server address failed.");
    }

    listen(server_fd, max_clients_);
    fds_.server_fd = server_fd;
}

void Server::acceptClient() {
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

int Server::getFirstFreeClient() {
    for (int i = 0; i < max_clients_; ++i)
        if (fds_.client_fds[i] != -1)
            return i;
    return -1;
}
