#include "check_username.hpp"

Response CheckUsernameHandler::callback(const Request& request) {
    std::string body = request.getBody();
    json body_json = json::parse(body);

    std::string username = body_json["username"];

}