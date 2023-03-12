#include "server_connector.hpp"

#include <arpa/inet.h>
#include <string.h>
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

std::string Connector::rcvMessage(int sock_fd) {
    int num_bytes = 0, cur_size = 0, total_size = 0;
    char buf[1024] = {0};
    char* result = NULL;
    for (;;) {
        num_bytes = recv(sock_fd, buf, 1024, 0);
        if (num_bytes <= 0)
            return std::string(result);
        total_size += num_bytes;
        result = (char*)realloc(result, cur_size + num_bytes);
        memcpy(&result[cur_size], buf, num_bytes);
        cur_size += num_bytes;
        if (num_bytes < 1024)
            break;
        num_bytes = 0;
    }
    result = (char*)realloc(result, total_size + 1);
    result[total_size] = '\0';
    return std::string(result);
}

void Connector::sendMessage(int sock_fd, const std::string& msg) {
    int left_bytes = msg.size();
    int tries = 0;
    const int max_tries = 10;
    for (; tries <= max_tries && left_bytes > 0; ++tries) {
        int sent_bytes = send(sock_fd, msg.c_str(), left_bytes, 0);
        if (sent_bytes == -1)
            throw InternalServerErr("tcp message send failed.");
        left_bytes -= sent_bytes;
    }
    if (left_bytes > 0)
        throw InternalServerErr("couldn't send tcp message completely.");
}

int Connector::acceptClient() {
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
    return client_fd;
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

int Connector::getFirstFreeClient() {
    for (int i = 0; i < connection_info_.max_clients; ++i)
        if (fds_.client_fds[i] != -1)
            return i;
    return -1;
}
