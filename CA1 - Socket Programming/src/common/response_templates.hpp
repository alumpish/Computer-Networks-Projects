#ifndef RESPONSE_TEMPLATES_HPP
#define RESPONSE_TEMPLATES_HPP

#include "response.hpp"

Response response311() {
    Response result;
    result.setBody("User signed up. Enter your password, purse, phone and address.");
    result.setStatus(311);
    return result;
}

Response response230() {
    Response result;
    result.setBody("User logged in.");
    result.setStatus(230);
    return result;
}

#endif