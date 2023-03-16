#include "view_user_handler.hpp"

Response ViewUserHandler::callback(const Request& request) {
    std::string username = hotel_->getUsername(request.getSessionID());
    json response_json = hotel_->getUserInfo(username);
    
    Response response;
    response.setBody(response_json.dump());
    return response;
}