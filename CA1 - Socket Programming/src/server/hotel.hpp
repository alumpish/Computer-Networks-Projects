#ifndef HOTEL_HPP
#define HOTEL_HPP

#include <fstream>
#include <unordered_map>

#include "date.h"
#include "json.hpp"
#include "types.hpp"
#include "timer.hpp"

class Hotel {
public:
    Hotel(const std::string& cur_date);
    void readUsers();
    void readRooms();
    bool isUserExist(const std::string& username);
    void addSession(const std::string& session_id, const std::string& username);
    void removeSession(const std::string& session_id);
    void addUser(User user);
    void signIn(const std::string& session_id, const std::string& username, const std::string& password);
    std::string getUsername(const std::string& session_id);
    json getUserInfo(const std::string& session_id);
    json getAllUsersInfo(const std::string& session_id);
    json getAllRoomsInfo(const std::string& session_id, date::sys_days cur_time);
    void bookRoom(const std::string& username, int room_num, int num_of_beds, date::sys_days check_in_date, date::sys_days check_out_date);
    json getReservations(const std::string& username);
    void cancelReservation(const std::string& username, int room_num, int count);
    void editInformation(const std::string& session_id, const std::string& password, const std::string& phone_number, const std::string& address);
    void editInformation(const std::string& session_id, const std::string& password);
    void passDay(const std::string& session_id, int days);
    void updateRooms(date::sys_days cur_date);
    void leaveRoom(const std::string& session_id, int room_num);
    void addRoom(const std::string& session_id, int room_num, int max_capacity, int price);
    void modifyRoom(const std::string& session_id, int room_num, int max_capacity, int price);
    void removeRoom(const std::string& session_id, int room_num);


private:
    Timer timer_;
    UserArray users_;
    RoomArray rooms_;
    std::unordered_map<std::string, std::string> sessions_un_map_;
};

#endif