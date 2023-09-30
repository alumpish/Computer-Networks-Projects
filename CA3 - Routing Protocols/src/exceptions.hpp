#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>

class CommandNotFound : public std::exception {
public:
    const char* what() const throw() {
        return "Command not found.";
    }
};

class IntraLinkDisallowed : public std::exception {
public:
    const char* what() const throw() {
        return "The source and destination of the link can't be the same.";
    }
};

class UndefinedLinkReference : public std::exception {
public:
    const char* what() const throw() {
        return "The referenced link does not exist.";
    }
};

#endif