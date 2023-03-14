#ifndef CHECK_USERNAME_HANDLER_HPP
#define CHECK_USERNAME_HANDLER_HPP

#include <string>

#include "request_handler.hpp"


class CheckUsernameHandler : public RequestHandler {
public:
    Response callback(const Request& request);

private:
    bool isPasswordValid(const std::string& password);
};

#endif