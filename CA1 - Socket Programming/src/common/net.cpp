#include "net.hpp"

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "exceptions.hpp"

int net::connectTCPServer(const std::string& host_name, int port, int max_clients) {
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(host_name.c_str());
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
        close(server_fd);
        throw ServerBindErr();
    }

    listen(server_fd, max_clients);
    return server_fd;
}

int net::connectTCPClient(const std::string& host_name, int port) {
    struct sockaddr_in server_address;

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(host_name.c_str());

    if (connect(sock_fd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        throw CLientConnectionErr();
    }
    return sock_fd;
}

std::string net::rcvMessage(int sock_fd) {
    int num_bytes = 0, cur_size = 0, total_size = 0;
    char buf[1024] = {0};
    char* result = nullptr;
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

void net::sendMessage(int sock_fd, std::string message) {
    int left_bytes = message.size();
    int tries = 0;
    const int max_tries = 10;
    for (; tries <= max_tries && left_bytes > 0; ++tries) {
        int sent_bytes = send(sock_fd, message.c_str(), left_bytes, 0);
        if (sent_bytes == -1)
            throw SendErr();
        left_bytes -= sent_bytes;
        message = message.substr(sent_bytes);
    }
    if (left_bytes > 0)
        throw SendErr();
}

int net::acceptClient(int server_fd) {
    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);

    client_fd = accept(server_fd, (struct sockaddr*)&client_address, (socklen_t*)&address_len);
    return client_fd;
}
