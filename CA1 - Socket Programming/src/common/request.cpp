#include "request.hpp"

#include <string>

#include "json.hpp"

using json = nlohmann::json;

Request::Request() {}

Request::Request(const std::string& request) {
    deserializeFromJSON(request);
}

void Request::setPath(std::string path) {
    header_.path = path;
}

void Request::setSessionID(std::string id) {
    header_.session_id = id;
}

void Request::setBody(std::string body) {
    body_ = body;
}

std::string Request::getPath() const {
    return header_.path;
}

std::string Request::getSessionID() const {
    return header_.session_id;
}

std::string Request::getBody() const {
    return body_;
}

std::string Request::toJSON() const {
    return serializeToJSON().dump();
}

json Request::serializeToJSON() const {
    json result = {};

    result["header"]["path"] = header_.path;
    result["header"]["session_id"] = header_.session_id;
    result["body"] = body_;

    return result;
}

void Request::deserializeFromJSON(const json& to_deserialize) {
    header_.path = to_deserialize["header"]["path"];
    header_.session_id = to_deserialize["header"]["session_id"];
    body_ = to_deserialize["body"];
}
