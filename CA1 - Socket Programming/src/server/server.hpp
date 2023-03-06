#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <vector>

class RequestHandler;

class Server {
public:
    Server() = delete;
    Server(const std::string& config_file = "./config.json");

    void run();
    void addHandler(RequestHandler* handler, const std::string& path);

private:
    int port_;
    std::string host_name_;
    int max_clients_;

    struct {
        int server_fd;
        std::vector<int> client_fds;
        fd_set master_fds_set;
        int max_fd;
    } fds_;

    void connectTCP();
    void acceptClient();

    int getFirstFreeClient();
};

#endif
