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
    while (true) {
        authenticate();
        if (user_type_ == UserType::admin)
            setupAdminCmds();
        else
            setupOrdinaryUserCmds();
        while (true) {
            std::cout << cmd_handler_.currentLevelCommandsToString();
            try {
                cmd_handler_.runSingleCommand();
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
                continue;
            }
            cmd_handler_.resetRoot();
            if (cmd_flags_.is_logged_out)
                break;
        }
        if (cmd_flags_.is_terminated)
            break;
    }
}

void Client::signin(const std::vector<std::string>& input_args) {
    json req_body = {};
    req_body["username"] = input_args[0];
    req_body["password"] = input_args[1];

    sendRequest(Consts::Paths::SIGNIN, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
    if (response.getStatus() == 230)
        cmd_flags_.authentication_finished = true;
}

void Client::signupUsername(const std::vector<std::string>& input_args) {
    json req_body = {};
    req_body["username"] = input_args[0];

    sendRequest(Consts::Paths::SIGNUP_USERNAME, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
    if (response.getStatus() != 311)
        return;

    std::cout << cas_cmd_handler_.currentLevelCommandsToString();
    cas_cmd_handler_.runSingleCommand();
}

void Client::signupUserInfo(const std::vector<std::string>& input_args) {
    json req_body = {};
    req_body["password"] = input_args[0];
    req_body["purse"] = std::stoi(input_args[1]);
    req_body["phone"] = input_args[2];
    req_body["address"] = input_args[3];

    sendRequest(Consts::Paths::SIGNUP_INFO, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
    if (response.getStatus() == 231)
        cmd_flags_.authentication_finished = true;
}

void Client::authenticate() {
    while (!cmd_flags_.authentication_finished) {
        cas_cmd_handler_.resetRoot();
        try {
            std::cout << cas_cmd_handler_.currentLevelCommandsToString();
            cas_cmd_handler_.runSingleCommand();
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            continue;
        }
    }
    setUserType();
    cmd_flags_.is_logged_out = false;
}

void Client::viewUserInformation(const std::vector<std::string>& input_args) {
    sendRequest(Consts::Paths::VIEW_USER_INFO, "");
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
}

void Client::viewAllUsers(const std::vector<std::string>& input_args) {
    sendRequest(Consts::Paths::VIEW_ALL_USERS, "");
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
}

void Client::viewRoomsInformation(const std::vector<std::string>& input_args) {
    sendRequest(Consts::Paths::VIEW_ROOMS_INFO, "");
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
}

void Client::booking(const std::vector<std::string>& input_args) {
    std::string room_num = input_args[0];
    std::string num_of_beds = input_args[1];
    std::string check_in_date = input_args[2];
    std::string check_out_date = input_args[3];

    json body = {};
    body["room_num"] = room_num;
    body["num_of_beds"] = num_of_beds;
    body["check_in_date"] = check_in_date;
    body["check_out_date"] = check_out_date;

    sendRequest(Consts::Paths::BOOKING, body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
}

void Client::canceling(const std::vector<std::string>& input_args) {
    sendRequest(Consts::Paths::VIEW_RESERVATIONS, "");
    Response reservations_response = Response(connector_.rcvMessage());
    std::cout << reservations_response.getBody();
    cmd_handler_.runSingleCommand();
}

void Client::cancelRoom(const std::vector<std::string>& input_args) {
    json req_body = {};
    req_body["room_num"] = input_args[0];
    req_body["num"] = input_args[1];

    sendRequest(Consts::Paths::CANCEL_ROOM, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
}

void Client::passDay(const std::vector<std::string>& input_args) {
    json req_body = {};
    req_body["value"] = input_args[0];

    sendRequest(Consts::Paths::PASS_DAY, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
}

void Client::editInformation(const std::vector<std::string>& input_args) {
    json req_body = {};
    req_body["pass"] = input_args[0];
    if (user_type_ == UserType::ordinary) {
        req_body["phone"] = input_args[1];
        req_body["address"] = input_args[2];
    }
    if (user_type_ == UserType::ordinary)
        sendRequest(Consts::Paths::EDIT_INFO, req_body.dump());
    else
        sendRequest(Consts::Paths::ADMIN_EDIT_INFO, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
}

void Client::leavingRoom(const std::vector<std::string>& input_args) {
    json req_body = {};
    req_body["room"] = input_args[0];
    if (user_type_ == UserType::admin)
        req_body["capacity"] = input_args[1];

    if (user_type_ == UserType::admin)
        sendRequest(Consts::Paths::ADMIN_LEAVING_ROOM, req_body.dump());
    else
        sendRequest(Consts::Paths::LEAVING_ROOM, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
}

void Client::addRoom(const std::vector<std::string>& input_args) {
    json req_body = {};
    req_body["room_num"] = input_args[0];
    req_body["max_capacity"] = input_args[1];
    req_body["price"] = input_args[2];

    sendRequest(Consts::Paths::ADD_ROOM, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
}

void Client::modifyRoom(const std::vector<std::string>& input_args) {
    json req_body = {};
    req_body["room_num"] = input_args[0];
    req_body["max_capacity"] = input_args[1];
    req_body["new_price"] = input_args[2];

    sendRequest(Consts::Paths::MODIFY_ROOM, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
}

void Client::removeRoom(const std::vector<std::string>& input_args) {
    json req_body = {};
    req_body["room_num"] = input_args[0];

    sendRequest(Consts::Paths::REMOVE_ROOM, req_body.dump());
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;
}

void Client::logout(const std::vector<std::string>& input_args) {
    sendRequest(Consts::Paths::LOGOUT, "");
    Response response = Response(connector_.rcvMessage());
    std::cout << response.getBody() << std::endl;

    cmd_flags_.is_logged_out = true;
    cmd_flags_.authentication_finished = false;
    // TODO close connection
}

void Client::terminate(const std::vector<std::string>& input_args) {
    cmd_flags_.is_terminated = true;
}

void Client::sendRequest(const std::string& path, const std::string& body) {
    Request req;
    req.setSessionID(session_id_);
    req.setPath(path);
    req.setBody(body);
    connector_.sendMessage(req.toJSON());
}

void Client::setUserType() {
    sendRequest(Consts::Paths::USER_TYPE, "");

    Response response = Response(connector_.rcvMessage());
    std::string type = response.getBody();

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
        new Command({".+", ".+"}, "signin <username> <password>", bind(&Client::signin)));

    cas_cmd_handler_.addCommand(
        "signup",
        new Command({".+"}, "signup <username>", bind(&Client::signupUsername)));

    cas_cmd_handler_["signup"].addCommand(
        "info",
        new Command(
            {".+", "\\d+", "^09\\d{9}$", ".+"},
            "info <password> <purse> <phone> <address>",
            bind(&Client::signupUserInfo)));
}

void Client::setupAdminCmds() {
    using Command = CommandHandler::Command;
    auto bind = [this](void (Client::*f)(const std::vector<std::string>&)) {
        return std::bind(f, this, std::placeholders::_1);
    };

    cmd_handler_.deleteCommands();

    cmd_handler_.addCommand("1", new Command({}, "View user information", bind(&Client::viewUserInformation)));

    cmd_handler_.addCommand("2", new Command({}, "View all users", bind(&Client::viewAllUsers)));

    cmd_handler_.addCommand("3", new Command({}, "View rooms information", bind(&Client::viewRoomsInformation)));

    cmd_handler_.addCommand("4", new Command({}, "Pass day", bind(&Client::dummyCommandNode)));
    cmd_handler_["4"].addCommand(
        "pass_day",
        new Command({"\\d+"}, "pass_day <value>", bind(&Client::passDay)));

    cmd_handler_.addCommand("5", new Command({}, "Edit Information", bind(&Client::dummyCommandNode)));
    cmd_handler_["5"].addCommand(
        "new_info",
        new Command(
            {".+"},
            "new_info <new password>",
            bind(&Client::editInformation)));

    cmd_handler_.addCommand("6", new Command({}, "Leaving room", bind(&Client::dummyCommandNode)));
    cmd_handler_["6"].addCommand(
        "room",
        new Command({"\\d+", "\\d+"}, "room <room number> <new capacity>", bind(&Client::leavingRoom)));

    cmd_handler_.addCommand("7", new Command({}, "Rooms", bind(&Client::dummyCommandNode)));
    cmd_handler_["7"].addCommand(
        "add",
        new Command({"\\d+", "\\d+", "\\d+"}, "add <room num> <capacity> <price>", bind(&Client::addRoom)));
    cmd_handler_["7"].addCommand(
        "modify",
        new Command({"\\d+", "\\d+", "\\d+"}, "modify <room num> <new capacity> <new price>", bind(&Client::modifyRoom)));
    cmd_handler_["7"].addCommand(
        "remove",
        new Command({"\\d+"}, "remove <room num>", bind(&Client::removeRoom)));

    cmd_handler_.addCommand("8", new Command({}, "Logout", bind(&Client::logout)));

    cmd_handler_.addCommand("9", new Command({}, "exit", bind(&Client::terminate)));
}

void Client::setupOrdinaryUserCmds() {
    using Command = CommandHandler::Command;
    auto bind = [this](void (Client::*f)(const std::vector<std::string>&)) {
        return std::bind(f, this, std::placeholders::_1);
    };

    cmd_handler_.deleteCommands();

    cmd_handler_.addCommand("1", new Command({}, "View user information", bind(&Client::viewUserInformation)));

    cmd_handler_.addCommand("2", new Command({}, "View rooms information", bind(&Client::viewRoomsInformation)));

    cmd_handler_.addCommand("3", new Command({}, "Booking", bind(&Client::dummyCommandNode)));
    cmd_handler_["3"].addCommand(
        "book",
        new Command(
            {"\\d+", "\\d+", Consts::Timer::DATE_FORMAT, Consts::Timer::DATE_FORMAT},
            "book <RoomNum> <NumOfBeds> <CheckInDate> <CheckoutDate>",
            bind(&Client::booking)));

    cmd_handler_.addCommand("4", new Command({}, "Canceling", bind(&Client::canceling)));
    cmd_handler_["4"].addCommand(
        "cancel", new Command({"\\d+", "\\d+"}, "cancel <RoomNum> <Num>", bind(&Client::cancelRoom)));

    cmd_handler_.addCommand("5", new Command({}, "Edit Information", bind(&Client::dummyCommandNode)));
    cmd_handler_["5"].addCommand(
        "new_info",
        new Command(
            {".+", "^09\\d{9}$", ".+"},
            "new_info <new password> <phone> <address>",
            bind(&Client::editInformation))

    );

    cmd_handler_.addCommand("6", new Command({}, "Leaving room", bind(&Client::dummyCommandNode)));
    cmd_handler_["6"].addCommand(
        "room",
        new Command({"\\d+"}, "room <room number>", bind(&Client::leavingRoom)));

    cmd_handler_.addCommand("7", new Command({}, "Logout", bind(&Client::logout)));

    cmd_handler_.addCommand("8", new Command({}, "exit", bind(&Client::terminate)));
}

void Client::dummyCommandNode(const std::vector<std::string>& input_args) {
    cmd_handler_.runSingleCommand();
}
