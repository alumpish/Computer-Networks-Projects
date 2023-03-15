#include "client.hpp"

#include <iostream>

#include "client_connector.hpp"
#include "command_handler.hpp"
#include "json.hpp"
#include "request.hpp"
#include "response.hpp"

using json = nlohmann::json;

Client::Client()
    : cmd_handler_(std::cin),
      connector_(8000, "127.0.0.1") {
    std::string init_response = connector_.rcvMessage();
    session_id_ = Response(init_response).getSessionID();
}

void Client::run() {
}

void Client::viewUserInformation(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    sendRequest(PATH, "");
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::viewAllUsers(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    sendRequest(PATH, "");
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::viewRoomsInformation(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    sendRequest(PATH, "");
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::booking(const std::vector<std::string>& input_args) {
    const std::string PATH = "";
    std::string room_num = input_args[0];
    std::string num_of_beds = input_args[1];
    std::string check_in_date = input_args[2];
    std::string check_out_date = input_args[3];

    json body = {};
    body["room_num"] = room_num;
    body["num_of_beds"] = num_of_beds;
    body["check_in_date"] = check_in_date;
    body["check_out_date"] = check_out_date;

    sendRequest(PATH, body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::canceling(const std::vector<std::string>& input_args) {
    const std::string RESERVATIONS_PATH = "";

    sendRequest(RESERVATIONS_PATH, "");
    Response reservations_response = Response(connector_.rcvMessage());
    std::cout << reservations_response.getBody();
    cmd_handler_.runSingleCommand();
}

void Client::cancelRoom(const std::vector<std::string>& input_args) {
    const std::string CANCELING_PATH = "";

    json req_body = {};
    req_body["room_num"] = input_args[0];
    req_body["num"] = input_args[1];

    sendRequest(CANCELING_PATH, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::passDay(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    json req_body = {};
    req_body["value"] = input_args[0];

    sendRequest(PATH, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::editInformation(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    json req_body = {};
    req_body["pass"] = input_args[0];
    if (user_type_ == UserType::ordinary) {
        req_body["phone"] = input_args[1];
        req_body["address"] = input_args[2];
    }

    sendRequest(PATH, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::leavingRoom(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    json req_body = {};
    req_body["room"] = input_args[0];
    if (user_type_ == UserType::admin)
        req_body["capacity"] = input_args[1];

    sendRequest(PATH, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::addRoom(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    json req_body = {};
    req_body["room_num"] = input_args[0];
    req_body["max_capacity"] = input_args[1];
    req_body["price"] = input_args[2];

    sendRequest(PATH, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::modifyRoom(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    json req_body = {};
    req_body["room_num"] = input_args[0];
    req_body["max_capacity"] = input_args[1];
    req_body["new_price"] = input_args[2];

    sendRequest(PATH, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::removeRoom(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    json req_body = {};
    req_body["room_num"] = input_args[0];

    sendRequest(PATH, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::logout(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    sendRequest(PATH, "");
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
    // TODO close connection
}

void Client::sendRequest(const std::string& path, const std::string& body) {
    Request req;
    req.setSessionID(session_id_);
    req.setPath(path);
    req.setBody(body);
    connector_.sendMessage(req.toJSON());
}
