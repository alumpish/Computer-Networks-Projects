#include "server_handlers.hpp"

#include <string>

#include "date.h"
#include "exceptions.hpp"
#include "request.hpp"
#include "response.hpp"
#include "response_templates.hpp"
#include "utils.hpp"

using json = nlohmann::json;

Response SignupUsernameHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());
    std::string username = req_body["username"];

    if (hotel_->isUserExist(username))
        throw Err451();

    hotel_->addSession(session_id, username);
    return response311();
}

Response SignupUserInfoHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());
    std::string password = req_body["password"];
    int purse = req_body["purse"];
    std::string phone_number = req_body["phone"];
    std::string address = req_body["address"];

    hotel_->signUp(session_id, password, purse, phone_number, address);
    return response231();
}

Response SigninHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());
    std::string username = req_body["username"];
    std::string password = req_body["password"];

    hotel_->signIn(session_id, username, password);
    return response230();
}

Response ViewUserInfoHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json users_info = hotel_->getUserInfo(session_id);

    Response result;
    result.setBody(users_info.dump());

    return result;
}

Response ViewAllUsersHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json users_list = hotel_->getAllUsersInfo(session_id);

    Response result;
    result.setBody(users_list.dump());

    return result;
}

Response ViewRoomsInfoHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json rooms_list = hotel_->getAllRoomsInfo(session_id);

    Response result;
    result.setBody(rooms_list.dump());

    return result;
}

Response BookingHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();
    const std::string& username = hotel_->getUsername(session_id);

    json req_body = json::parse(request.getBody());
    int room_num = req_body["room_num"];
    int num_of_beds = req_body["num_of_beds"];
    date::sys_days check_in_date;
    parse(req_body["check_in_date"], check_in_date);
    date::sys_days check_out_date;
    parse(req_body["check_out_date"], check_out_date);

    hotel_->bookRoom(username, room_num, num_of_beds, check_in_date, check_out_date);

    Response result;
    // TODO set body

    return result;
}

Response ViewReservationsHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();
    const std::string& username = hotel_->getUsername(session_id);

    json reservations_list = hotel_->getReservations(username);

    Response result;
    result.setBody(reservations_list.dump());

    return result;
}

Response CancelRoomHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();
    const std::string& username = hotel_->getUsername(session_id);

    json req_body = json::parse(request.getBody());
    int room_num = req_body["room_num"];
    int count = req_body["num"];

    hotel_->cancelReservation(username, room_num, count);

    Response result;

    return result;
}

Response PassDayHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());
    int days = req_body["value"];

    hotel_->passDay(session_id, days);

    Response result;

    return result;
}

Response AdminEditInformationHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());

    std::string password = req_body["pass"];

    hotel_->editInformation(session_id, password);

    Response result;

    return result;
}

Response EditInformationHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());

    std::string password = req_body["pass"];
    std::string phone = req_body["phone"];
    std::string address = req_body["address"];

    hotel_->editInformation(session_id, password, phone, address);

    Response result;

    return result;
}

Response LeavingRoomHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());

    int room_num = req_body["room"];

    hotel_->leaveRoom(session_id, room_num);

    Response result;

    return result;
}

Response AdminLeavingRoomHandler::handleResponse(const Request& request) {
    Response result;
    result.setSessionID(request.getSessionID());
    result.setBody("Not implemented.");
    return result;
}

Response AddRoomHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());
    int room_num = req_body["room_num"];
    int max_capacity = req_body["max_capacity"];
    int price = req_body["price"];

    hotel_->addRoom(session_id, room_num, max_capacity, price);

    Response result;

    return result;
}

Response ModifyRoomHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());
    int room_num = req_body["room_num"];
    int max_capacity = req_body["max_capacity"];
    int price = req_body["price"];

    hotel_->modifyRoom(session_id, room_num, max_capacity, price);

    Response result;

    return result;
}

Response RemoveRoomHandler::handleResponse(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());
    int room_num = req_body["room_num"];

    hotel_->removeRoom(session_id, room_num);

    Response result;

    return result;
}

Response LogoutHandler::handleResponse(const Request& request) {
}

Response GetUserTypeHandler::handleResponse(const Request& request) {
    const std::string session_id = request.getSessionID();

    User::Type user_type = hotel_->getUserType(session_id);
    
    Response result;
    if (user_type == User::Type::ordinary)
        result.setBody("ordinary_user");
    else
        result.setBody("admin");

    return result;
}
