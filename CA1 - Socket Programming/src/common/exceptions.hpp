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

class Err101 : std::exception {
    const char* what() const throw() {
        return "101: The desired room was not found.";
    }
};

class Err102 : std::exception {
    const char* what() const throw() {
        return "102: Your reservation was not found.";
    }
};

class Err104 : std::exception {
    const char* what() const throw() {
        return "Successfully added.";
    }
};

class Err105 : std::exception {
    const char* what() const throw() {
        return "Successfully modified.";
    }
};

class Err106 : std::exception {
    const char* what() const throw() {
        return "Successfully deleted.";
    }
};

class Err108 : std::exception {
    const char* what() const throw() {
        return "108: Your account balance is not enough.";
    }
};

class Err109 : std::exception {
    const char* what() const throw() {
        return "109: The room capacity is full.";
    }
};

class Err110 : std::exception {
    const char* what() const throw() {
        return "110: Succcessfully done.";
    }
};

class Err111 : std::exception {
    const char* what() const throw() {
        return "111: This room already exists.";
    }
};

class Err 201 : std::exception{
                    const char * what() const throw(){
                                     return "201: User logged out successfully.";
}
}
;

class Err230 : std::exception {
    const char* what() const throw() {
        return "230: User logged in.";
    }
};

class Err231 : std::exception {
    const char* what() const throw() {
        return "231: User successfully signed up.";
    }
};

class Err311 : std::exception {
    const char* what() const throw() {
        return "311: User signed up. Enter your password, purse, phone and address.";
    }
};

class Err312 : std::exception {
    const char* what() const throw() {
        return "312: Information was changed successfully.";
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

class Err412 : std::exception {
    const char* what() const throw() {
        return "412: Invalid capacity value!";
    }
};

class Err413 : std::exception {
    const char* what() const throw() {
        return "413: Successfully leaving!";
    }
};

class Err430 : std::exception {
    const char* what() const throw() {
        return "430: Invalid username or password!";
    }
};

class Err451 : std::exception {
    const char* what() const throw() {
        return "User already existed!";
    }
};

class Err503 : std::exception {
    const char* what() const throw() {
        return "503: Bad sequence of commands.";
    }
};

#endif