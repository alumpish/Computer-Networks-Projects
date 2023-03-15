#include "client_connector.hpp"

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdexcept>

Connector::Connector(int port, const std::string& host_name) {
    struct sockaddr_in server_address;

    sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(host_name.c_str());

    if (connect(sock_fd_, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        throw std::runtime_error("Error connecting to server.");
    }
}

void Connector::sendMessage(const std::string& msg) {
    int left_bytes = msg.size();
    int tries = 0;
    const int max_tries = 10;
    for (; tries <= max_tries && left_bytes > 0; ++tries) {
        int sent_bytes = send(sock_fd_, msg.c_str(), left_bytes, 0);
        if (sent_bytes == -1)
            throw std::runtime_error("tcp message send failed.");
        left_bytes -= sent_bytes;
    }
    if (left_bytes > 0)
        throw std::runtime_error("couldn't send tcp message completely.");
}

std::string Connector::rcvMessage() {
    int num_bytes = 0, cur_size = 0, total_size = 0;
    char buf[1024] = {0};
    char* result = NULL;
    for (;;) {
        num_bytes = recv(sock_fd_, buf, 1024, 0);
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
