#include "view_user_handler.hpp"

Response ViewUserHandler::callback(const Request& request) {
    std::string username = hotel_->getUsername(request.getSessionId());
    User user = hotel_->users_.getUser(username);

    json response_json;
    response_json["user"] = user.username;
    response_json["admin"] = user.is_admin;
    response_json["purse"] = user.purse;
    response_json["phoneNumber"] = user.phone_number;
    response_json["address"] = user.address;

    Response response;
    response.setBody(response_json.dump());
    return response;
}