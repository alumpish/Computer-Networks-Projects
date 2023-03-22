#include "utils.hpp"


bool isNumber(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    for (char c : str) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

bool isPasswordValid(const std::string& password) {
    if (password.empty()) {
        return false;
    }
    for (char c : password) {
        if (isspace(c)) {
            return false;
        }
    }
    return true;
}
