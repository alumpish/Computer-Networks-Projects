#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "hotel.hpp"
#include "json.hpp"
#include "request.hpp"
#include "response.hpp"

class RequestHandler {
public:
    Response callback(const Request& request);
protected:
    virtual Response handleResponse(const Request& request) = 0;
};

#endif