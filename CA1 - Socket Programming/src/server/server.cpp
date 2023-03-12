#include "server.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cmath>
#include <ctime>
#include <fstream>
#include <random>
#include <unordered_map>
#include <unordered_set>

#include "exceptions.hpp"
#include "json.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "response.hpp"
#include "server_connector.hpp"

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
    auto event = connector_.pollForEvent();
    if (event.type == Connector::Event::EventType::incoming_client)
        handleIncomingClient(event);
    else if (event.type == Connector::Event::EventType::stdin_cmd)
        handleSTDINCommand(event);
    else if (event.type == Connector::Event::EventType::client_req)
        handleIncomingRequest(event);
}

void Server::addHandler(RequestHandler* handler, const std::string& path) {
    handlers_map_[path] = handler;
}

void Server::handleIncomingClient(Connector::Event event) {
    if (event.type != Connector::Event::EventType::incoming_client)
        return;

    int new_client_fd = connector_.acceptClient();

    std::string new_session_id = genSessionID();
    sessions_.insert(new_session_id);

    Response response;
    response.setSessionID(new_session_id);
    connector_.sendMessage(new_client_fd, response.toJSON());
}

void Server::handleIncomingRequest(Connector::Event event) {
    if (event.type != Connector::Event::EventType::client_req)
        return;

    std::string request_string = connector_.rcvMessage(event.sock_fd);
    Request req(request_string);

    if (!isAuthorized(req.getSessionID()))
        throw Err403();

    RequestHandler* handler = findRequestHandler(req.getPath());
    Response response = handler->callback(req);
    response.setSessionID(req.getSessionID());
    connector_.sendMessage(event.sock_fd, response.toJSON());
}

void Server::handleSTDINCommand(Connector::Event event) {
    if (event.type != Connector::Event::EventType::stdin_cmd)
        return;

    // TODO handle stdin commands
}

std::string Server::genSessionID() const {
    const std::string LETTERS =
        "1234567890abcdefghijklmnopqrstuvxyz";
    const int ID_LEN = 20;

    std::mt19937 rng;
    rng.seed(std::time(nullptr));
    std::uniform_int_distribution<int> uint_dist(0, ID_LEN);

    std::string result;
    result.reserve(ID_LEN);
    for (int i = 0; i < ID_LEN; ++i) {
        result += LETTERS[uint_dist(rng)];
    }
    return result;
}

RequestHandler* Server::findRequestHandler(const std::string& path) {
    const auto& found_handler = handlers_map_.find(path);
    if (found_handler == handlers_map_.end())
        throw InternalServerErr("Requested path not found.");

    return found_handler->second;
}

bool Server::isAuthorized(const std::string& session_id) {
    return sessions_.find(session_id) != sessions_.end();
}
