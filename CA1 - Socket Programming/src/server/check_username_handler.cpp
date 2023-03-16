#include "check_username_handler.hpp"

Response CheckUsernameHandler::callback(const Request& request) {
    std::string body = request.getBody();
    json body_json = json::parse(body);

    std::string username = body_json["username"];

    if (hotel_->isUserExist(username)) {
        throw Err451();
    }

    hotel_->addSession(request.getSessionID(), username);

    // Todo: send response to client to send other information
    Response response;
    return response;
}