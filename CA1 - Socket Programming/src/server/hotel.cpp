#include "hotel.hpp"

Hotel::Hotel(Timer& timer) : timer_(timer) {
    readUsers();
    readRooms();
}

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

void Hotel::addSession(const std::string& session_id, const std::string& username) {
    sessions_un_map_[session_id] = username;
}

void Hotel::removeSession(const std::string& session_id) {
    sessions_un_map_.erase(session_id);
}

void Hotel::signUp(const std::string& session_id, const std::string& password, int purse, const std::string& phone_number, const std::string& address) {
    if (!isPasswordValid(password) || !isNumber(phone_number)){
        removeSession(session_id);
        throw Err503();
    }
    
    User new_user(getUsername(session_id), password, false, purse, phone_number, address);
    users_.addUser(new_user);
}

void Hotel::signIn(const std::string& session_id, const std::string& username, const std::string& password) {
    for (auto user : users_.users) {
        if (user.username == username && user.password == password) {
            addSession(session_id, username);
            return;
        }
    }
    throw Err430();
}

std::string Hotel::getUsername(const std::string& session_id) {
    return sessions_un_map_[session_id];
}

json Hotel::getUserInfo(const std::string& session_id) {
    std::string username = getUsername(session_id);
    User* user = users_.getUser(username);

    json user_json;
    user_json["id"] = user->id;
    user_json["user"] = user->username;
    user_json["password"] = user->password;

    if (user->type == User::Type::admin)
        user_json["type"] = "admin";
    else
        user_json["type"] = "ordinary";

    user_json["purse"] = user->purse;
    user_json["phoneNumber"] = user->phone_number;
    user_json["address"] = user->address;
    return user_json;
}

json Hotel::getAllUsersInfo(const std::string& session_id) {
    std::string username = getUsername(session_id);

    if (users_.getUser(username)->type == User::Type::ordinary) {
        throw Err403();
    }

    json users_json;
    for (auto user : users_.users) {
        json user_json;
        user_json["id"] = user.id;
        user_json["username"] = user.username;

        if (user.type == User::Type::admin)
            user_json["type"] = "admin";
        else
            user_json["type"] = "ordinary";

        user_json["purse"] = user.purse;
        user_json["phoneNumber"] = user.phone_number;
        user_json["address"] = user.address;
        users_json.push_back(user_json);
    }
    return users_json;
}

json Hotel::getAllRoomsInfo(const std::string& session_id) {
    std::string username = getUsername(session_id);

    json rooms_json;
    for (auto room : rooms_.rooms) {
        json room_json;
        room_json["number"] = room.number;
        room_json["status"] = room.status;
        room_json["price"] = room.price;
        room_json["maxCapacity"] = room.max_capacity;
        room_json["capacity"] = room.getCapacity(timer_.getCurrentDate());

        if (users_.getUser(username)->type == User::Type::admin) {
            json reservations_json;
            for (auto reservation : room.reservations) {
                json reservation_json;
                reservation_json["id"] = reservation.user_id;
                reservation_json["numOfBeds"] = reservation.num_of_beds;
                reservation_json["checkInDate"] = to_string(reservation.check_in_date);
                reservation_json["checkOutDate"] = to_string(reservation.check_out_date);
                reservations_json.push_back(reservation_json);
            }
            room_json["users"] = reservations_json;
        }

        rooms_json.push_back(room_json);
    }
    return rooms_json;
}

void Hotel::bookRoom(const std::string& username, int room_num, int num_of_beds, date::sys_days check_in_date, date::sys_days check_out_date) {
    User* user = users_.getUser(username);
    Room* room = rooms_.getRoom(room_num);

    if (user->purse < room->price * num_of_beds) {
        throw Err108();
    }
    if (room->isAvailable(num_of_beds, check_in_date, check_out_date) == false) {
        throw Err109();
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
            if (reservation.user_id == user->id) {
                json reservation_json;
                reservation_json["roomNumber"] = room.number;
                reservation_json["numOfBeds"] = reservation.num_of_beds;
                reservation_json["checkInDate"] = to_string(reservation.check_in_date);
                reservation_json["checkOutDate"] = to_string(reservation.check_out_date);
                reservations_json.push_back(reservation_json);
            }
        }
    }
    return reservations_json;
}

void Hotel::cancelReservation(const std::string& username, int room_num, int count) {
    User* user = users_.getUser(username);
    Room* room = rooms_.getRoom(room_num);

    room->removeReservation(user->id, count, timer_.getCurrentDate());
    user->purse += (room->price * count) / 2;
    return;
}

void Hotel::editInformation(const std::string& session_id, const std::string& password, const std::string& phone_number, const std::string& address) {
    std::string username = getUsername(session_id);
    User* user = users_.getUser(username);
    user->editInformation(password, phone_number, address);
    return;
}

void Hotel::editInformation(const std::string& session_id, const std::string& password) {
    std::string username = getUsername(session_id);
    User* user = users_.getUser(username);
    user->editInformation(password);
    return;
}

void Hotel::passDay(const std::string& session_id, int days) {
    std::string username = getUsername(session_id);
    if (users_.getUser(username)->type == User::Type::ordinary) {
        throw Err403();
    }

    timer_.addDays(days);
    updateRooms(timer_.getCurrentDate());
}

void Hotel::updateRooms(date::sys_days cur_date) {
    for (auto room : rooms_.rooms) {
        room.updateReservations(cur_date);
    }
}

void Hotel::leaveRoom(const std::string& session_id, int room_num) {
    std::string username = getUsername(session_id);
    User* user = users_.getUser(username);
    Room* room = rooms_.getRoom(room_num);

    room->leaveRoom(user, timer_.getCurrentDate());
    return;
}

void Hotel::addRoom(const std::string& session_id, int room_num, int max_capacity, int price) {
    std::string username = getUsername(session_id);
    if (users_.getUser(username)->type == User::Type::ordinary) {
        throw Err403();
    }
    rooms_.addRoom(room_num, max_capacity, price);
    return;
}

void Hotel::modifyRoom(const std::string& session_id, int room_num, int max_capacity, int price) {
    std::string username = getUsername(session_id);
    if (users_.getUser(username)->type == User::Type::ordinary) {
        throw Err403();
    }
    rooms_.modifyRoom(room_num, max_capacity, price);
    return;
}

void Hotel::removeRoom(const std::string& session_id, int room_num) {
    std::string username = getUsername(session_id);
    if (users_.getUser(username)->type == User::Type::ordinary) {
        throw Err403();
    }
    rooms_.removeRoom(room_num);
    return;
}

void Hotel::logOut(const std::string& session_id) {
    std::string username = getUsername(session_id);
    removeSession(username);
    return;
}

User::Type Hotel::getUserType(const std::string& session_id) {
    std::string username = getUsername(session_id);
    return users_.getUser(username)->type;
}
