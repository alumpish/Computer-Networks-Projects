#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "hotel.hpp"
#include "json.hpp"
#include "response.hpp"
#include "request.hpp"

// class Request;
// class Response;

class RequestHandler {
public:
    RequestHandler(Hotel* hotel) : hotel_(hotel) {}
    virtual Response callback(const Request& request) = 0;

protected:
    Hotel* hotel_;
};

#endif