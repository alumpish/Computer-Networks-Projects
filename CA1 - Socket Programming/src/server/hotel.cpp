#include "hotel.hpp"

void Hotel::readUsers() {
    std::ifstream users_file("usersInfo.json");
    json users_json;
    users_file >> users_json;
    for (auto user : users_json) {
        User new_user;
        new_user.username = user["user"];
        new_user.password = user["password"];
        if (user["admin"]; == "true") {
            new_user.is_admin = true;
        }
        else {
            new_user.is_admin = false;
            new_user.purse = user["purse"];
            new_user.phone_number = user["phone"];
            new_user.address = user["address"];
        }
        users_.addUser(new_user);
    }
}

void Hotel::readRooms() {
    std::ifstream rooms_file("roomsInfo.json");
    json rooms_json;
    rooms_file >> rooms_json;
    for (auto room : rooms_json) {
        Room new_room;
        new_room.id = room["number"];
        new_room.status = room["status"];
        new_room.price = room["price"];
        new_room.max_capacity = room["maxCapacity"];
        new_room.capacity = room["capacity"];
        new_room.is_available = room["is_available"];
        rooms_.addRoom(new_room);
    }
}
