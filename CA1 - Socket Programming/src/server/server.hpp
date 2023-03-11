#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

#include "server_connector.hpp"

class RequestHandler;

class Server {
public:
    Server() = delete;
    Server(const std::string& config_file = "./config.json");

    void run();
    void addHandler(RequestHandler* handler, const std::string& path);

private:
    Connector connector_;
};

#endif
