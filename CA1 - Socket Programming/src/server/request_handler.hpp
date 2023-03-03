#ifndef REQUESTHANDLER_HPP
#define REQUESTHANDLER_HPP

class Request;
class Response;

class RequestHandler {
public:
    virtual Response callback(const Request& request) = 0;
};

#endif