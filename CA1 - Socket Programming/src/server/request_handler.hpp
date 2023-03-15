#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "hotel.hpp"
#include "json.hpp"

class Request;
class Response;

class RequestHandler {
public:
    virtual Response callback(const Request& request) = 0;
    RequestHandler(Hotel* hotel) : hotel_(hotel);

protected:
    Hotel* hotel_;
};

#endif