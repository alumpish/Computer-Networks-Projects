#include "signup_handler.hpp"

Response SignupHandler::callback(const Request& request) {
    int purse;
    std::string body = request.getBody();
    json body_json = json::parse(body);
    std::string password = body_json["password"];
    std::string phone = body_json["phoneNumber"];
    std::string address = body_json["address"];

    try {
        purse = body_json["purse"];
    }
    catch (const std::exception& e) {
        throw Err503;
    }

    if (!isPasswordValid(password) || !isNumber(phone)) {
        hotel_->removeSession(request.getSessionId());
        throw Err503;
    }

    std::string username = hotel_->getUsername(request.getSessionId());
    User new_user(username, password, false, purse, phone, address);
    hotel_->users_.addUser(new_user);
}

bool isPasswordValid(const std::string& password) {
    if (password.empty()) {
        return false;
    }
    for (char c : password) {
        if (isspace(c)) {
            return false;
        }
    }
    return true;
}