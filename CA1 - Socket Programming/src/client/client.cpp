#include "client.hpp"

#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "client_connector.hpp"
#include "command_handler.hpp"
#include "consts.hpp"
#include "json.hpp"
#include "request.hpp"
#include "response.hpp"

using json = nlohmann::json;

Client::Client()
    : cas_cmd_handler_(std::cin),
      cmd_handler_(std::cin),
      connector_(8000, "127.0.0.1") {
    std::string init_response = connector_.rcvMessage();
    session_id_ = Response(init_response).getSessionID();
    setupCASCmds();
}

void Client::run() {
    authenticate();
    if (user_type_ == UserType::admin)
        setupAdminCmds();
    else
        setupOrdinaryUserCmds();
    while (true) {
        cmd_handler_.runSingleCommand();
        cmd_handler_.resetRoot();
    }
}

void Client::signin(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    json req_body = {};
    req_body["username"] = input_args[0];
    req_body["password"] = input_args[1];

    sendRequest(PATH, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::signupUsername(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    json req_body = {};
    req_body["username"] = input_args[0];

    sendRequest(PATH, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
    if (response.getStatus() != 311)
        return;

    cas_cmd_handler_.runSingleCommand();
}

void Client::signupUserInfo(const std::vector<std::string>& input_args) {
    const std::string PATH = "";

    json req_body = {};
    req_body["password"] = input_args[0];
    req_body["purse"] = input_args[1];
    req_body["phone"] = input_args[2];
    req_body["address"] = input_args[3];

    sendRequest(PATH, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody();
}

void Client::authenticate() {
    cas_cmd_handler_.runSingleCommand();
    setUserType();
    cas_cmd_handler_.resetRoot();
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

void Client::setUserType() {
    const std::string PATH = "";

    sendRequest(PATH, "");

    Response response = Response(connector_.rcvMessage());
    std::string type = json::parse(response.getBody())["type"];

    if (type == "ordinary_user")
        user_type_ = UserType::ordinary;
    else
        user_type_ = UserType::admin;
}

void Client::setupCASCmds() {
    using Command = CommandHandler::Command;
    auto bind = [this](void (Client::*f)(const std::vector<std::string>&)) {
        return std::bind(f, this, std::placeholders::_1);
    };

    cas_cmd_handler_.addCommand(
        "signin",
        new Command({".+", ".+"}, "signin <username> <password>", bind(&Client::signin))
    );

    cas_cmd_handler_.addCommand(
        "signup",
        new Command({".+"}, "signup <username>", bind(&Client::signupUsername))
    );
    cas_cmd_handler_["signup"].addCommand(
        "info",
        new Command(
            {".+", "\\d+", "^09\\d{9}$", ".+"},
            "info <password> <purse> <phone> <address>",
            bind(&Client::signupUserInfo)
        )
    );
}

void Client::setupAdminCmds() {
    using Command = CommandHandler::Command;
    auto bind = [this](void (Client::*f)(const std::vector<std::string>&)) {
        return std::bind(f, this, std::placeholders::_1);
    };

    cmd_handler_.addCommand("1", new Command({}, "View user information", bind(&Client::viewUserInformation)));

    cmd_handler_.addCommand("2", new Command({}, "View all users", bind(&Client::viewAllUsers)));

    cmd_handler_.addCommand("3", new Command({}, "View rooms information", bind(&Client::viewRoomsInformation)));

    cmd_handler_.addCommand("4", new Command({}, "Pass day", bind(&Client::dummyCommandNode)));
    cmd_handler_["4"].addCommand(
        "pass_day",
        new Command({"\\d+"}, "pass_day <value>", bind(&Client::passDay))
    );

    cmd_handler_.addCommand("5", new Command({}, "Edit Information", bind(&Client::dummyCommandNode)));
    cmd_handler_["5"].addCommand(
        "new_info",
        new Command(
            {".+"},
            "new_info <new password>",
            bind(&Client::editInformation)
        )
    );

    cmd_handler_.addCommand("6", new Command({}, "Leaving room", bind(&Client::dummyCommandNode)));
    cmd_handler_["6"].addCommand(
        "room",
        new Command({"\\d+", "\\d+"}, "room <room number> <new capacity>", bind(&Client::leavingRoom))
    );

    cmd_handler_.addCommand("7", new Command({}, "Rooms", bind(&Client::dummyCommandNode)));
    cmd_handler_["7"].addCommand(
        "add",
        new Command({"\\d+", "\\d+", "\\d+"}, "add <room num> <capacity> <price>", bind(&Client::addRoom))
    );
    cmd_handler_["7"].addCommand(
        "modify",
        new Command({"\\d+", "\\d+", "\\d+"}, "modify <room num> <new capacity> <new price>", bind(&Client::modifyRoom))
    );
    cmd_handler_["7"].addCommand(
        "remove",
        new Command({"\\d+"}, "remove <room num>", bind(&Client::removeRoom))
    );

    cmd_handler_.addCommand("8", new Command({}, "Logout", bind(&Client::logout)));
}

void Client::setupOrdinaryUserCmds() {
    using Command = CommandHandler::Command;
    auto bind = [this](void (Client::*f)(const std::vector<std::string>&)) {
        return std::bind(f, this, std::placeholders::_1);
    };

    cmd_handler_.addCommand("1", new Command({}, "View user information", bind(&Client::viewUserInformation)));

    cmd_handler_.addCommand("2", new Command({}, "View rooms information", bind(&Client::viewRoomsInformation)));

    cmd_handler_.addCommand("3", new Command({}, "Booking", bind(&Client::dummyCommandNode)));
    cmd_handler_["3"].addCommand(
        "book",
        new Command(
            {"\\d+", "\\d+", Consts::Timer::DATE_FORMAT, Consts::Timer::DATE_FORMAT},
            "book <RoomNum> <NumOfBeds> <CheckInDate> <CheckoutDate>",
            bind(&Client::booking)
        )
    );

    cmd_handler_.addCommand("4", new Command({}, "Canceling", bind(&Client::canceling)));
    cmd_handler_["4"].addCommand(
        "cancel", new Command({"\\d+", "\\d+"}, "cancel <RoomNum> <Num>", bind(&Client::cancelRoom))
    );

    cmd_handler_.addCommand("5", new Command({}, "Edit Information", bind(&Client::dummyCommandNode)));
    cmd_handler_["5"].addCommand(
        "new_info",
        new Command(
            {".+", "^09\\d{9}$", ".+"},
            "new_info <new password> <phone> <address>",
            bind(&Client::editInformation)
        )

    );

    cmd_handler_.addCommand("6", new Command({}, "Leaving room", bind(&Client::dummyCommandNode)));
    cmd_handler_["6"].addCommand(
        "room",
        new Command({"\\d+"}, "room <room number>", bind(&Client::leavingRoom))
    );

    cmd_handler_.addCommand("7", new Command({}, "Logout", bind(&Client::logout)));
}

void Client::dummyCommandNode(const std::vector<std::string>& input_args) {
    cmd_handler_.runSingleCommand();
}
