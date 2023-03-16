#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "hotel.hpp"
#include "json.hpp"
#include "request.hpp"
#include "response.hpp"

class RequestHandler {
public:
    virtual Response callback(const Request& request) = 0;
};

#endif