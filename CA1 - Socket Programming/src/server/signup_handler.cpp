#include "signup_handler.hpp"

Response SignupHandler::callback(const Request& request) {
    std::string body = request.getBody();
    json body_json = json::parse(body);
    std::string password = body_json["password"];
    std::string phone = body_json["phone"];
    std::string address = body_json["address"];

    try {
        int purse = body_json["purse"];
    }
    catch (const std::exception& e) {
        throw Err503;
    }

    if (!isPasswordValid(password) || !isNumber(phone)) {
        throw Err503;
    }

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