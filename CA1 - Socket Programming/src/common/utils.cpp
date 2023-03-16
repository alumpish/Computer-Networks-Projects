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

bool parse(const std::string& date, date::sys_days& res) {
    date::sys_days ymd;
    std::istringstream ss(date);
    ss >> date::parse("%d-%m-%Y", ymd);
    if (ss.fail()) {
        return false;
    }
    res = ymd;
    return true;
}

std::string to_string(date::sys_days d) {
    std::ostringstream ss;
    ss << date::format("%d-%m-%Y", d);
    return ss.str();
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
