#ifndef RESPONSE_TEMPLATES_HPP
#define RESPONSE_TEMPLATES_HPP

#include "response.hpp"

Response response104() {
    Response result;
    result.setBody("Successfully added.");
    result.setStatus(104);
    return result;
}

Response response105() {
    Response result;
    result.setBody("Successfully modified.");
    result.setStatus(105);
    return result;
}

Response response106() {
    Response result;
    result.setBody("Successfully deleted.");
    result.setStatus(106);
    return result;
}

Response response110() {
    Response result;
    result.setBody("Successfully done.");
    result.setStatus(110);
    return result;
}

Response response201() {
    Response result;
    result.setBody("User logged out successfully.");
    result.setStatus(201);
    return result;
}

Response response230() {
    Response result;
    result.setBody("User logged in.");
    result.setStatus(230);
    return result;
}

Response response231() {
    Response result;
    result.setBody("User successfully signed up.");
    result.setStatus(231);
    return result;
}

Response response311() {
    Response result;
    result.setBody("User signed up. Enter your password, purse, phone and address.");
    result.setStatus(311);
    return result;
}

Response response312() {
    Response result;
    result.setBody("Information was changed successfully.");
    result.setStatus(312);
    return result;
}

Response response413() {
    Response result;
    result.setBody("Successfully leaving.");
    result.setStatus(413);
    return result;
}

#endif