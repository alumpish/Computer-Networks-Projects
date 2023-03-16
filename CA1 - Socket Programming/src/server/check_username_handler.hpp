#ifndef CHECK_USERNAME_HANDLER_HPP
#define CHECK_USERNAME_HANDLER_HPP

#include <string>

#include "exceptions.hpp"
#include "request_handler.hpp"


class CheckUsernameHandler : public RequestHandler {
public:
    CheckUsernameHandler(Hotel* hotel) : RequestHandler(hotel) {}
    Response callback(const Request& request);
};

#endif