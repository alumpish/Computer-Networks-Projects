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
    int port = data["port"];
    std::string host_name = data["hostName"];
    int max_clients = data["maxClients"];

    connector_ = Connector(port, host_name, max_clients);
}

void Server::run() {
}

void Server::addHandler(RequestHandler* handler, const std::string& path) {
}
