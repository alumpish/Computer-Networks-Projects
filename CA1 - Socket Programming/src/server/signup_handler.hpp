#ifndef SIGNUP_HANDLER_HPP
#define SIGNUP_HANDLER_HPP

#include <string>

#include "../common/exceptions.hpp"
#include "../common/utils.hpp"
#include "request_handler.hpp"

class SignupHandler : public RequestHandler {
public:
    SignupHandler(Hotel* hotel) : RequestHandler(hotel) {}
    Response callback(const Request& request);

private:
    bool isPasswordValid(const std::string& password);
};

#endif