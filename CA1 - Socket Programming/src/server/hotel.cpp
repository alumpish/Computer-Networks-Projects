#include "hotel.hpp"

void Hotel::readUsers() {
    std::ifstream users_file("usersInfo.json");
    json users_json;
    users_file >> users_json;
    for (auto user : users_json) {
        User new_user(user);
        users_.addUser(new_user);
    }
}

void Hotel::readRooms() {
    std::ifstream rooms_file("roomsInfo.json");
    json rooms_json;
    rooms_file >> rooms_json;
    for (auto room : rooms_json) {
        Room new_room(room);
        rooms_.addRoom(new_room);
    }
}
