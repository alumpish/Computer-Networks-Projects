#ifndef EXCEPTIONS_CPP
#define EXCEPTIONS_CPP

#include <stdexcept>

class Err503 : std::exception {
    const char* what() const throw() {
        return "503: Bad sequence of commands.";
    }
};

class Err401 : std::exception {
    const char* what() const throw () {
        return "401: Invalid value!";
    }
};

#endif