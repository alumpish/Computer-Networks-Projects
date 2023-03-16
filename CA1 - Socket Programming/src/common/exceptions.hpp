#ifndef EXCEPTIONS_CPP
#define EXCEPTIONS_CPP

#include <stdexcept>
#include <string>

class CustomException : public std::exception {
public:
    CustomException()
        : status_code_(0) {}
    CustomException(int status_code)
        : status_code_(status_code) {}

    int getStatus() const { return status_code_; }

private:
    int status_code_;
};

class InternalServerErr : public CustomException {
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

class CommandNotFound : CustomException {
    const char* what() const throw() {
        return "Command not found.";
    }
};

class Err101 : public CustomException {
public:
    Err101()
        : CustomException(101) {}

    const char* what() const throw() {
        return "101: The desired room was not found.";
    }
};

class Err102 : public CustomException {
public:
    Err102()
        : CustomException(102) {}

    const char* what() const throw() {
        return "102: Your reservation was not found.";
    }
};

class Err104 : public CustomException {
public:
    Err104()
        : CustomException(104) {}

    const char* what() const throw() {
        return "Successfully added.";
    }
};

class Err105 : public CustomException {
public:
    Err105()
        : CustomException(105) {}

    const char* what() const throw() {
        return "Successfully modified.";
    }
};

class Err106 : public CustomException {
public:
    Err106()
        : CustomException(106) {}

    const char* what() const throw() {
        return "Successfully deleted.";
    }
};

class Err108 : public CustomException {
public:
    Err108()
        : CustomException(108) {}

    const char* what() const throw() {
        return "108: Your account balance is not enough.";
    }
};

class Err109 : public CustomException {
public:
    Err109()
        : CustomException(109) {}

    const char* what() const throw() {
        return "109: The room capacity is full.";
    }
};

class Err110 : public CustomException {
public:
    Err110()
        : CustomException(110) {}

    const char* what() const throw() {
        return "110: Succcessfully done.";
    }
};

class Err111 : public CustomException {
public:
    Err111()
        : CustomException(111) {}

    const char* what() const throw() {
        return "111: This room already exists.";
    }
};

class Err201 : public CustomException {
public:
    Err201()
        : CustomException(201) {}

    const char* what() const throw() {
        return "201: User logged out successfully.";
    }
};

class Err230 : public CustomException {
public:
    Err230()
        : CustomException(230) {}

    const char* what() const throw() {
        return "230: User logged in.";
    }
};

class Err231 : public CustomException {
public:
    Err231()
        : CustomException(231) {}

    const char* what() const throw() {
        return "231: User successfully signed up.";
    }
};

class Err311 : public CustomException {
public:
    Err311()
        : CustomException(311) {}

    const char* what() const throw() {
        return "311: User signed up. Enter your password, purse, phone and address.";
    }
};

class Err312 : public CustomException {
public:
    Err312()
        : CustomException(312) {}

    const char* what() const throw() {
        return "312: Information was changed successfully.";
    }
};

class Err401 : public CustomException {
public:
    Err401()
        : CustomException(401) {}

    const char* what() const throw() {
        return "401: Invalid value!";
    }
};

class Err403 : public CustomException {
public:
    Err403()
        : CustomException(403) {}

    const char* what() const throw() {
        return "Access denied!";
    }
};

class Err412 : public CustomException {
public:
    Err412()
        : CustomException(412) {}

    const char* what() const throw() {
        return "412: Invalid capacity value!";
    }
};

class Err413 : public CustomException {
public:
    Err413()
        : CustomException(413) {}

    const char* what() const throw() {
        return "413: Successfully leaving!";
    }
};

class Err430 : public CustomException {
public:
    Err430()
        : CustomException(430) {}

    const char* what() const throw() {
        return "430: Invalid username or password!";
    }
};

class Err451 : public CustomException {
public:
    Err451()
        : CustomException(451) {}

    const char* what() const throw() {
        return "User already existed!";
    }
};

class Err503 : public CustomException {
public:
    Err503()
        : CustomException(503) {}

    const char* what() const throw() {
        return "503: Bad sequence of commands.";
    }
};

#endif