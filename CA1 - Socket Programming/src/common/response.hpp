#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

#include "json.hpp"

using json = nlohmann::json;

class Response {
public:
    Response();
    Response(const std::string& response);

    void setSessionID(std::string id);
    void setStatus(int status_code);
    void setBody(std::string body);

    std::string getSessionID() const;
    int getStatus() const;
    std::string getBody() const;

    std::string toJSON() const;

private:
    struct {
        std::string session_id;
        int status_code;
    } header_;

    std::string body_;

    json serializeToJSON() const;
    void deserializeFromJSON(const json& to_deserialize);
};

#endif