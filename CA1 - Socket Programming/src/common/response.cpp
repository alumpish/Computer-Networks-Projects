#include "response.hpp"

#include <string>

#include "json.hpp"

using json = nlohmann::json;

Response::Response(const std::string& response) {
    deserializeFromJSON(response);
}

void Response::setSessionID(std::string id) {
    header_.session_id = id;
}

void Response::setStatus(int status_code) {
    header_.status_code = status_code;
}

void Response::setBody(std::string body) {
    body_ = body;
}

std::string Response::getSessionID() const {
    return header_.session_id;
}


int Response::getStatus() const {
    return header_.status_code;
}

std::string Response::getBody() const {
   return body_; 
}

std::string Response::toJSON() const {
    return serializeToJSON().dump();
}

json Response::serializeToJSON() const {
    json result = {};

    result["header"]["session_id"] = header_.session_id;
    result["header"]["status"] = header_.status_code;
    result["body"] = body_;

    return result;
}

void Response::deserializeFromJSON(const json& to_deserialize) {
    header_.status_code = to_deserialize["header"]["status"];
    header_.session_id = to_deserialize["header"]["session_id"];
    body_ = to_deserialize["body"];
}
