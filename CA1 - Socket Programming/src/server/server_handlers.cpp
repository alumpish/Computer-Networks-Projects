#include "server_handlers.hpp"

#include <string>

#include "date.h"
#include "request.hpp"
#include "response.hpp"
#include "utils.hpp"

Response SignupUsernameHandler::callback(const Request& request) {
}

Response SignupUserInfoHandler::callback(const Request& request) {
}

Response SigninHandler::callback(const Request& request) {
}

Response ViewUserInfoHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json users_info = hotel_->getUserInfo(session_id);

    Response result;
    result.setSessionID(session_id);
    result.setBody(users_info.dump());

    return result;
}

Response ViewAllUsersHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json users_list = hotel_->getAllUsersInfo(session_id);

    Response result;
    result.setSessionID(session_id);
    result.setBody(users_list.dump());

    return result;
}

Response ViewRoomsInfoHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json rooms_list = hotel_->getAllRoomsInfo(session_id);

    Response result;
    result.setSessionID(session_id);
    result.setBody(rooms_list.dump());

    return result;
}

Response BookingHandler::callback(const Request& request) {
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
    result.setSessionID(session_id);
    // TODO set body

    return result;
}

Response ViewReservationsHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();
    const std::string& username = hotel_->getUsername(session_id);

    json reservations_list = hotel_->getReservations(username);

    Response result;
    result.setSessionID(session_id);
    result.setBody(reservations_list.dump());

    return result;
}

Response CancelRoomHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();
    const std::string& username = hotel_->getUsername(session_id);

    json req_body = json::parse(request.getBody());
    int room_num = req_body["room_num"];
    int count = req_body["num"];

    hotel_->cancelReservation(username, room_num, count);

    Response result;
    result.setSessionID(session_id);

    return result;
}

Response PassDayHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());
    int days = req_body["value"];

    hotel_->passDay(session_id, days);

    Response result;
    result.setSessionID(session_id);

    return result;
}

Response AdminEditInformationHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());

    std::string password = req_body["pass"];

    hotel_->editInformation(session_id, password);

    Response result;
    result.setSessionID(session_id);

    return result;
}

Response EditInformationHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());

    std::string password = req_body["pass"];
    std::string phone = req_body["phone"];
    std::string address = req_body["address"];

    hotel_->editInformation(session_id, password, phone, address);

    Response result;
    result.setSessionID(session_id);

    return result;
}

Response LeavingRoomHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());

    int room_num = req_body["room"];

    hotel_->leaveRoom(session_id, room_num);

    Response result;
    result.setSessionID(session_id);

    return result;
}

Response AddRoomHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());
    int room_num = req_body["room_num"];
    int max_capacity = req_body["max_capacity"];
    int price = req_body["price"];

    hotel_->addRoom(session_id, room_num, max_capacity, price);

    Response result;
    result.setSessionID(session_id);

    return result;
}

Response ModifyRoomHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());
    int room_num = req_body["room_num"];
    int max_capacity = req_body["max_capacity"];
    int price = req_body["price"];

    hotel_->modifyRoom(session_id, room_num, max_capacity, price);

    Response result;
    result.setSessionID(session_id);

    return result;
}

Response RemoveRoomHandler::callback(const Request& request) {
    const std::string& session_id = request.getSessionID();

    json req_body = json::parse(request.getBody());
    int room_num = req_body["room_num"];

    hotel_->removeRoom(session_id, room_num);

    Response result;
    result.setSessionID(session_id);

    return result;
}

Response LogoutHandler::callback(const Request& request) {
}
