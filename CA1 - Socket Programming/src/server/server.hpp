#ifndef SERVER_HPP
#define SERVER_HPP

#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "command_handler.hpp"
#include "logger.hpp"
#include "server_connector.hpp"
#include "server_timer.hpp"
#include "timer.hpp"

class RequestHandler;

class Server {
public:
    Server() = delete;
    Server(const std::string& config_file, Timer& timer, ServerTimer& server_timer);

    void run();
    void addHandler(RequestHandler* handler, const std::string& path);

    ~Server();

private:
    Connector connector_;
    CommandHandler cmd_handler_;

    std::unordered_set<std::string> sessions_;
    std::unordered_map<std::string, RequestHandler*> handlers_map_;

    ServerTimer& timer_;

    Logger logger_;
    std::ofstream log_file_;

    bool exit_ = false;

    void handleIncomingClient(Connector::Event event);
    void handleIncomingRequest(Connector::Event event);
    void handleSTDINCommand(Connector::Event event);

    std::string genSessionID() const;
    RequestHandler* findRequestHandler(const std::string& path);
    bool isAuthorized(const std::string& session_id);
    void setupCommands();

    void setTime(const std::vector<std::string>& input_args);
    void exit(const std::vector<std::string>& input_args);
};

#endif
