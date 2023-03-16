#ifndef VIEW_USER_HANDLER_HPP
#define VIEW_USER_HANDLER_HPP

#include "request_handler.hpp"
#include "types.hpp"
#include "json.hpp"

class ViewUserHandler : public RequestHandler {
public:
    ViewUserHandler(Hotel* hotel) : RequestHandler(hotel) {}
    Response callback(const Request& request);
};

#endif