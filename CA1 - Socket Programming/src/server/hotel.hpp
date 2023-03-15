#ifndef HOTEL_HPP
#define HOTEL_HPP

#include <unordered_map>

#include "../common/types.hpp"
#include "json.hpp"


class Hotel {
    public:
    void readUsers();
    void readRooms();
    bool isUserExist(const std::string& username);
    void addSession(const std::string& session_id, std::string username);
    void Hotel::removeSession(const std::string& session_id)
    std::string getUsername(const std::string& session_id);
    json Hotel::getAllUsersInfo();
    json Hotel::getAllRoomsInfo();


    private:
    UserArray users_;
    RoomArray rooms_;
    std::unordered_map<std::string, std::string> sessions_un_map_;
}

#endif