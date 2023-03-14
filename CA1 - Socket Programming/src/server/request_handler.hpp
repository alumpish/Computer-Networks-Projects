#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

#include "json.hpp"
#include "hotel.hpp"

class Request;
class Response;

class RequestHandler {
public:
    RequestHandler(Hotel* hotel) : hotel_(hotel);
    virtual Response callback(const Request& request) = 0;

private:
    Hotel* hotel_;
};

#endif