#include "view_users_handler.hpp"

Response ViewUsersHandler::callback(const Request& request) {
    json response_json = hotel_->getAllUsersInfo();
    Response response;
    response.setBody(response_json.dump());
    return response;
}