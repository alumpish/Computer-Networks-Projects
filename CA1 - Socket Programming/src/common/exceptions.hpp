#ifndef EXCEPTIONS_CPP
#define EXCEPTIONS_CPP

#include <stdexcept>
#include <string>

class InternalServerErr : std::exception {
public:
    InternalServerErr(std::string desc)
        : desc_(desc) {}

    const char* what() const throw() {
        std::string msg = "Internal error : " + msg;
        return msg.c_str();
    }

private:
    const std::string desc_;
};

class Err503 : std::exception {
    const char* what() const throw() {
        return "503: Bad sequence of commands.";
    }
};

class Err401 : std::exception {
    const char* what() const throw() {
        return "401: Invalid value!";
    }
};

class Err403 : std::exception {
    const char* what() const throw() {
        return "Access denied!";
    }
};

#endif