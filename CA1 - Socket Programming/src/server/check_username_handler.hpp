#ifndef CHECK_USERNAME_HANDLER_HPP
#define CHECK_USERNAME_HANDLER_HPP

#include <string>

#include "../common/exeptions.hpp"
#include "request_handler.hpp"

class CheckUsernameHandler : public RequestHandler {
public:
    CheckUsernameHandler(Hotel* hotel) : hotel_(hotel) {}
    Response callback(const Request& request);

private:
};

#endif