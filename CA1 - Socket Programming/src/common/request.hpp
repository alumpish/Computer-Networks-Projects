#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <string>

#include "json.hpp"

using json = nlohmann::json;

class Request {
public:
    Request();
    Request(const std::string& request);

    void setPath(std::string path);
    void setSessionID(std::string id);
    void setBody(std::string body);

    std::string getPath() const;
    std::string getSessionID() const;
    std::string getBody() const;

    std::string toJSON() const;

private:
    struct {
        std::string path;
        std::string session_id;
    } header_;

    std::string body_;

    json serializeToJSON() const;
    void deserializeFromJSON(const json& to_deserialize);
};

#endif