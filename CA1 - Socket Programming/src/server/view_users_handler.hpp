#ifndef VIEW_USERS_HANDLER_HPP
#define VIEW_USERS_HANDLER_HPP

#include "request_handler.hpp"
#include "types.hpp"
#include "json.hpp"

class ViewUsersHandler : public RequestHandler {
public:
    ViewUsersHandler(Hotel* hotel) : RequestHandler(hotel) {}
    Response callback(const Request& request);
};

#endif