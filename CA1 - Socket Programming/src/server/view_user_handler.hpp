#ifndef VIEW_USER_HANDLER_HPP
#define VIEW_USER_HANDLER_HPP

#include "../common/types.hpp"
#include "json.hpp"

class ViewUserHandler : public RequestHandler {
public:
    ViewUserHandler(Hotel* hotel) : hotel_(hotel) {}
    Response* callback(Request* req);
};

#endif