#ifndef HOTEL_HPP
#define HOTEL_HPP

#include <unordered_map>
#include <fstream>

#include "types.hpp"
#include "date.h"
#include "json.hpp"

class Hotel {
public:
    void readUsers();
    void readRooms();
    bool isUserExist(const std::string& username);
    void addSession(const std::string& session_id, const std::string& username);
    void removeSession(const std::string& session_id);
    void addUser(User user);
    std::string getUsername(const std::string& session_id);
    json getUserInfo(const std::string& username);
    json getAllUsersInfo();
    json getAllRoomsInfo(date::sys_days cur_time);
    void bookRoom(const std::string& username, int room_num, int num_of_beds, date::sys_days check_in_date, date::sys_days check_out_date);
    json getReservations(const std::string& username);
    void cancelReservation(const std::string& username, int room_num, int count);
    void editInformation(const std::string& username, const std::string& password, const std::string& phone_number, const std::string& address);

private:
    UserArray users_;
    RoomArray rooms_;
    std::unordered_map<std::string, std::string> sessions_un_map_;
};

#endif