#include "client.hpp"

#include <sys/stat.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "client_connector.hpp"
#include "command_handler.hpp"
#include "consts.hpp"
#include "json.hpp"
#include "logger.hpp"
#include "request.hpp"
#include "response.hpp"
#include "timer.hpp"
#include "types.hpp"

using json = nlohmann::json;
using args_list = const std::vector<std::string>&;

Client::Client(Timer& timer)
    : cas_cmd_handler_(std::cin),
      cmd_handler_(std::cin),
      connector_(8000, "127.0.0.1"),
      timer_(timer),
      logger_(timer) {
    std::string init_response = connector_.rcvMessage();
    session_id_ = Response(init_response).getSessionID();
    setupCASCmds();
}

void Client::run() {
    do {
        authenticate();
        if (user_type_ == UserType::admin)
            setupAdminCmds();
        else
            setupOrdinaryUserCmds();
        runCmds();
    } while (!cmd_flags_.is_terminated);
}

void Client::signIn(args_list input_args) {
    json req_body = {};
    req_body["username"] = input_args[0];
    req_body["password"] = input_args[1];

    sendRequest(Consts::Paths::SIGNIN, req_body.dump());
    Response response = rcvResponse();
    std::cout << response.getBody() << std::endl;
    if (response.getStatus() == 230)
        cmd_flags_.authentication_finished = true;
    username_ = input_args[0];
}

void Client::signupUsername(args_list input_args) {
    json req_body = {};
    req_body["username"] = input_args[0];

    sendRequest(Consts::Paths::SIGNUP_USERNAME, req_body.dump());
    Response response = rcvResponse();
    std::cout << response.getBody() << std::endl;
    if (response.getStatus() != 311)
        return;

    std::cout << cas_cmd_handler_.currentLevelCommandsToString();
    username_ = input_args[0];
    cas_cmd_handler_.runSingleCommand();
}

void Client::signupUserInfo(args_list input_args) {
    json req_body = {};
    req_body["password"] = input_args[0];
    req_body["purse"] = std::stoi(input_args[1]);
    req_body["phone"] = input_args[2];
    req_body["address"] = input_args[3];

    sendRequest(Consts::Paths::SIGNUP_INFO, req_body.dump());
    Response response = rcvResponse();
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
    mkdir(username_.c_str(), 0777);
    log_file_ = new std::ofstream(username_ + "/log.txt", std::ios::app);
    logger_.setStream(log_file_);
}

void Client::viewUserInformation(args_list input_args) {
    sendRequest(Consts::Paths::VIEW_USER_INFO, "");
    Response response = rcvResponse();
    json res_body = json::parse(response.getBody());

    std::cout << User::jsonToString(res_body);
}

void Client::viewAllUsers(args_list input_args) {
    sendRequest(Consts::Paths::VIEW_ALL_USERS, "");
    Response response = rcvResponse();
    json res_body = json::parse(response.getBody());

    for (json user_json : res_body) {
        std::cout << User::jsonSafeViewToString(user_json);
    }
}

void Client::viewRoomsInformation(args_list input_args) {
    sendRequest(Consts::Paths::VIEW_ROOMS_INFO, "");

    Response response = rcvResponse();
    json res_body = json::parse(response.getBody());

    std::vector<Room> rooms_list;
    for (auto& room : res_body)
        rooms_list.push_back(Room(room));
    if (rooms_list.empty()) {
        std::cout << "No room to show." << std::endl;
        return;
    }

    for (const auto& room : rooms_list)
        std::cout << room.toString() << std::endl;
}

void Client::booking(args_list input_args) {
    std::string room_num = input_args[0];
    std::string num_of_beds = input_args[1];
    std::string check_in_date = input_args[2];
    std::string check_out_date = input_args[3];

    json body = {};
    body["room_num"] = std::stoi(room_num);
    body["num_of_beds"] = std::stoi(num_of_beds);
    body["check_in_date"] = check_in_date;
    body["check_out_date"] = check_out_date;

    sendRequest(Consts::Paths::BOOKING, body.dump());
    Response response = Response(connector_.rcvMessage());
    logger_.info("Received response: " + response.toJSON(), false);
    std::cout << response.getBody() << std::endl;
}

void Client::canceling(args_list input_args) {
    sendRequest(Consts::Paths::VIEW_RESERVATIONS, "");
    Response reservations_response = rcvResponse();
    json res_body = json::parse(reservations_response.getBody());

    if (res_body.empty())
        std::cout << "No reservation to show." << std::endl;

    for (auto reservation : res_body)
        std::cout << Reservation(reservation).toString() << std::endl;

    cmd_handler_.runSingleCommand();
}

void Client::cancelRoom(args_list input_args) {
    json req_body = {};
    req_body["room_num"] = input_args[0];
    req_body["num"] = input_args[1];

    sendRequest(Consts::Paths::CANCEL_ROOM, req_body.dump());
    Response response = rcvResponse();
    std::cout << response.getBody() << std::endl;
}

void Client::passDay(args_list input_args) {
    json req_body = {};
    req_body["value"] = std::stoi(input_args[0]);

    sendRequest(Consts::Paths::PASS_DAY, req_body.dump());
    Response response = rcvResponse();
    std::cout << response.getBody() << std::endl;
}

void Client::editInformation(args_list input_args) {
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
    Response response = rcvResponse();
    std::cout << response.getBody() << std::endl;
}

void Client::leavingRoom(args_list input_args) {
    json req_body = {};
    req_body["room"] = std::stoi(input_args[0]);
    if (user_type_ == UserType::admin)
        req_body["capacity"] = std::stoi(input_args[1]);

    if (user_type_ == UserType::admin)
        sendRequest(Consts::Paths::ADMIN_LEAVING_ROOM, req_body.dump());
    else
        sendRequest(Consts::Paths::LEAVING_ROOM, req_body.dump());
    Response response = rcvResponse();
    std::cout << response.getBody() << std::endl;
}

void Client::addRoom(args_list input_args) {
    json req_body = {};
    req_body["room_num"] = std::stoi(input_args[0]);
    req_body["max_capacity"] = std::stoi(input_args[1]);
    req_body["price"] = std::stoi(input_args[2]);

    sendRequest(Consts::Paths::ADD_ROOM, req_body.dump());
    Response response = rcvResponse();
    std::cout << response.getBody() << std::endl;
}

void Client::modifyRoom(args_list input_args) {
    json req_body = {};
    req_body["room_num"] = std::stoi(input_args[0]);
    req_body["max_capacity"] = std::stoi(input_args[1]);
    req_body["new_price"] = std::stoi(input_args[2]);

    sendRequest(Consts::Paths::MODIFY_ROOM, req_body.dump());
    Response response = rcvResponse();
    std::cout << response.getBody() << std::endl;
}

void Client::removeRoom(args_list input_args) {
    json req_body = {};
    req_body["room_num"] = std::stoi(input_args[0]);

    sendRequest(Consts::Paths::REMOVE_ROOM, req_body.dump());
    Response response = rcvResponse();
    std::cout << response.getBody() << std::endl;
}

void Client::logout(args_list input_args) {
    sendRequest(Consts::Paths::LOGOUT, "");
    Response response = rcvResponse();
    std::cout << response.getBody() << std::endl;

    cmd_flags_.is_logged_out = true;
    cmd_flags_.authentication_finished = false;
    resetConnection();
}

void Client::terminate(args_list input_args) {
    cmd_flags_.is_terminated = true;
}

void Client::sendRequest(const std::string& path, const std::string& body) {
    Request req;
    req.setSessionID(session_id_);
    req.setPath(path);
    req.setBody(body);
    connector_.sendMessage(req.toJSON());
}

Response Client::rcvResponse() {
    std::string response_str = connector_.rcvMessage();
    logger_.info("Received response: " + response_str, false);
    return Response(response_str);
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
        new Command({".+", ".+"}, "signin <username> <password>", bind(&Client::signIn))
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

    cmd_handler_.deleteCommands();

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

    cmd_handler_.addCommand("8", new Command({}, "exit", bind(&Client::terminate)));
}

void Client::resetConnection() {
    connector_.closeConnection();
    connector_ = Connector(8000, "127.0.0.1");
    std::string init_response = connector_.rcvMessage();
    session_id_ = Response(init_response).getSessionID();
}

void Client::runCmds() {
    do {
        std::cout << cmd_handler_.currentLevelCommandsToString();
        try {
            cmd_handler_.runSingleCommand();
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
            logger_.error(e.what(), false);
            continue;
        }
        cmd_handler_.resetRoot();
    } while (!cmd_flags_.is_logged_out);
}

void Client::dummyCommandNode(const std::vector<std::string>& input_args) {
    std::cout << cmd_handler_.currentLevelCommandsToString();
    cmd_handler_.runSingleCommand();
}
