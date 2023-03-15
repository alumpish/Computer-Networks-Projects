#ifndef HOTEL_HPP
#define HOTEL_HPP

#include <unordered_map>

#include "../common/types.hpp"
#include "date/date.h"
#include "json.hpp"

class Hotel {
public:
    void readUsers();
    void readRooms();
    bool isUserExist(const std::string& username);
    void addSession(const std::string& session_id, std::string username);
    void removeSession(const std::string& session_id);
    std::string getUsername(const std::string& session_id);
    json getUserInfo(const std::string& username);
    json getAllUsersInfo();
    json getAllRoomsInfo();
    void bookRoom(const std::string& username, int room_num, int num_of_beds, data::sys_days check_in_date, data::sys_days check_out_date);
    json getReservations(const std::string& username);
    void cancelReservation(const std::string& username, int room_num, int count);
    void editInormation(const std::string& username, std::string password, std::string phone_number, std::string address);

private:
    UserArray users_;
    RoomArray rooms_;
    std::unordered_map<std::string, std::string> sessions_un_map_;
}

#endif