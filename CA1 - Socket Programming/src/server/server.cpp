#include "server.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "consts.hpp"
#include "exceptions.hpp"
#include "json.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "response.hpp"
#include "server_connector.hpp"
#include "timer.hpp"

using json = nlohmann::json;

Server::Server(const std::string& config_file, Timer& timer)
    : cmd_handler_(std::cin),
      timer_(timer),
      logger_(timer) {
    std::ifstream f(config_file);
    json data = json::parse(f);
    int port = data["port"];
    std::string host_name = data["hostName"];
    int max_clients = data["maxClients"];

    connector_ = Connector(port, host_name, max_clients);

    log_file_ = std::ofstream("./log.txt");
    logger_.setStream(&log_file_);

    setupCommands();
}

void Server::run() {
    std::cout << cmd_handler_.currentLevelCommandsToString() << std::endl;
    cmd_handler_.runSingleCommand();

    while (!exit_) {
        try {
            auto event = connector_.pollForEvent();
            if (event.type == Connector::Event::EventType::incoming_client)
                handleIncomingClient(event);
            else if (event.type == Connector::Event::EventType::stdin_cmd)
                handleSTDINCommand(event);
            else if (event.type == Connector::Event::EventType::client_req)
                handleIncomingRequest(event);
        }
        catch (std::exception& e) {
            logger_.error(e.what());
        }
    }
}

void Server::addHandler(RequestHandler* handler, const std::string& path) {
    handlers_map_[path] = handler;
}

Server::~Server() {
    logger_.info("Server shutdown.");
    log_file_.close();
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

    logger_.info("New client accepted.");
    logger_.footer("Session ID: " + new_session_id);
}

void Server::handleIncomingRequest(Connector::Event event) {
    if (event.type != Connector::Event::EventType::client_req)
        return;

    std::string request_string;
    try {
        request_string = connector_.rcvMessage(event.sock_fd);
    }
    catch (...) {
        connector_.removeConnection(event.sock_fd);
        logger_.info("Connection closed on socket " + std::to_string(event.sock_fd) + ".");
        return;
    }

    Request req(request_string);

    logger_.info("Incoming request: ");
    logger_.footer("Path: " + req.getPath());
    logger_.footer(req.getBody());
    logger_.footer("Session ID: " + req.getSessionID());

    if (!isAuthorized(req.getSessionID()))
        throw Err403();

    RequestHandler* handler = findRequestHandler(req.getPath());
    Response response = handler->callback(req);
    response.setSessionID(req.getSessionID());
    connector_.sendMessage(event.sock_fd, response.toJSON());

    logger_.info("Outgoing response: ");
    logger_.footer(response.getBody());
    logger_.footer(response.getSessionID());
}

void Server::handleSTDINCommand(Connector::Event event) {
    if (event.type != Connector::Event::EventType::stdin_cmd)
        return;

    cmd_handler_.runSingleCommand();
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

void Server::setupCommands() {
    using Command = CommandHandler::Command;
    auto bind = [this](void (Server::*f)(const std::vector<std::string>&)) {
        return std::bind(f, this, std::placeholders::_1);
    };

    cmd_handler_.addCommand(
        "initTime",
        new Command({Consts::Timer::DATE_FORMAT}, "initTime <date>", bind(&Server::setTime)));

    cmd_handler_["initTime"].addCommand(
        "exit",
        new Command({}, "exit", bind(&Server::exit)));

    cmd_handler_["initTime"].addCommand(
        "setTime",
        new Command({Consts::Timer::DATE_FORMAT}, "setTime <date>", bind(&Server::setTime)));
}

void Server::setTime(const std::vector<std::string>& input_args) {
    std::string current_time = input_args[0];

    timer_.setTime(current_time);
}

void Server::exit(const std::vector<std::string>& input_args) {
    exit_ = true;
}
