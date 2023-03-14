#ifndef SIGNUP_HANDLER_HPP
#define SIGNUP_HANDLER_HPP

#include <string>

#include "request_handler.hpp"
#include "../common/utils.hpp"
#include "../common/exceptions.hpp"


class SignupHandler : public RequestHandler {
public:
    Response callback(const Request& request);

private:
    bool isPasswordValid(const std::string& password);
};

#endif