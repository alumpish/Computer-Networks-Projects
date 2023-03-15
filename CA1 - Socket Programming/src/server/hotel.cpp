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

bool Hotel::isUserExist(const std::string& username) {
    for (auto user : users_.users) {
        if (user.username == username) {
            return true;
        }
    }
    return false;
}

void Hotel::addSession(const std::string& session_id, std::string username) {
    sessions_un_map_[session_id] = username;
}

void Hotel::removeSession(const std::string& session_id) {
    sessions_un_map_.erase(session_id);
}

std::string Hotel::getUsername(const std::string& session_id) {
    return sessions_un_map_[session_id];
}

json Hotel::getUserInfo(const std::string& username) {
    json user_json;
    User* user = users_.getUser(username);

    user_json["id"] = user->id;
    user_json["user"] = user->username;
    user_json["password"] = user->password;
    user_json["admin"] = user->is_admin;
    user_json["purse"] = user->purse;
    user_json["phoneNumber"] = user->phone_number;
    user_json["address"] = user->address;
    return user_json;
}

json Hotel::getAllUsersInfo() {
    json users_json;
    for (auto user : users_.users) {
        json user_json;
        user_json["id"] = user.id;
        user_json["username"] = user.username;
        user_json["admin"] = user.is_admin;
        user_json["purse"] = user.purse;
        user_json["phoneNumber"] = user.phone_number;
        user_json["address"] = user.address;
        users_json.push_back(user_json);
    }
    return users_json;
}

json Hotel::getAllRoomsInfo() {
    json rooms_json;
    for (auto room : rooms_.rooms) {
        json room_json;
        room_json["number"] = room.number;
        room_json["status"] = room.status;
        room_json["price"] = room.price;
        room_json["maxCapacity"] = room.max_capacity;
        room_json["capacity"] = room.capacity;

        json reservations_json;
        for (auto reservation : room.reservations) {
            json reservation_json;
            reservation_json["id"] = reservation.id;
            reservation_json["numOfBeds"] = reservation.num_of_beds;
            // todo: add reserve_date and check_out_date

            reservations_json.push_back(reservation_json);
        }
        room_json["users"] = reservations_json;

        rooms_json.push_back(room_json);
    }
    return rooms_json;
}

void bookRoom(const std::string& username, int room_num, int num_of_beds, , std::string check_in_date, std::string check_out_date) {
    User* user = users_.getUser(username);
    Room* room = rooms_.getRoom(room_num);

    date::sys_days check_in_date_ = date::parse("%Y-%m-%d", check_in_date);
    date::sys_days check_out_date_ = date::parse("%Y-%m-%d", check_out_date);

    if (user->purse < room->price * num_of_beds) {
        throw PermissionDenied();
    }
    if (room->isAvailable(num_of_beds, check_in_date, check_out_date) == false) {
        throw PermissionDenied();
    }

    user->purse -= room->price * num_of_beds;

    Reservation new_reservation(user->id, num_of_beds, check_in_date, check_out_date);
    room->addReservation(new_reservation);
}

json Hotel::getReservations(const std::string& username) {
    User* user = users_.getUser(username);
    json reservations_json;
    for (auto room : rooms_.rooms) {
        for (auto reservation : room.reservations) {
            if (reservation.id == user->id) {
                json reservation_json;
                reservation_json["roomNumber"] = room.number;
                reservation_json["numOfBeds"] = reservation.num_of_beds;
                reservation_json["checkInDate"] = reservation.check_in_date;
                reservation_json["checkOutDate"] = reservation.check_out_date;
                reservations_json.push_back(reservation_json);
            }
        }
    }
    return reservations_json;
}

void cancelReservation(const std::string& username, int room_num, int count) {
    User* user = users_.getUser(username);
    Room* room = rooms_.getRoom(room_num);

    room->removeReservation(user->id, count);
}
