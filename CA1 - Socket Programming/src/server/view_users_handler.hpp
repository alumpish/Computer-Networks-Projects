#ifndef VIEW_USERS_HANDLER_HPP
#define VIEW_USERS_HANDLER_HPP

#include "../common/types.hpp"
#include "json.hpp"

class ViewUsersHandler : public RequestHandler {
public:
    ViewUsersHandler(Hotel* hotel) : hotel_(hotel) {}
    Response* callback(Request* req);
};

#endif