#ifndef SERVER_HPP
#define SERVER_HPP

class RequestHandler;

class Server {
public:
    void run();
    void addHandler(RequestHandler* handler);
private:
};


#endif