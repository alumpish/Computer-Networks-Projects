#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>

#include "date.h"
#include "exceptions.hpp"
#include "json.hpp"
#include "utils.hpp"

using json = nlohmann::json;

struct User {
    User(std::string username, std::string password, bool is_admin, int purse, std::string phone_number, std::string address) {
        this->username = username;
        this->password = password;
        if (is_admin)
            type = User::Type::admin;
        else
            type = User::Type::ordinary;
        this->purse = purse;
        this->phone_number = phone_number;
        this->address = address;
    }
    User(json user_json) {
        id = user_json["id"];
        username = user_json["user"];
        password = user_json["password"];

        if (user_json["admin"]) {
            type = User::Type::admin;
            return;
        }
        type = User::Type::ordinary;

        purse = user_json["purse"];
        phone_number = user_json["phoneNumber"];
        address = user_json["address"];
    }

    enum class Type {
        admin,
        ordinary
    } type;

    int id;
    std::string username;
    std::string password;
    int purse;
    std::string phone_number;
    std::string address;

    void editInformation(std::string password, std::string phone_number, std::string address) {
        if (!password.empty()) {
            if (!isPasswordValid(password))
                throw Err503();
            this->password = password;
        }
        if (!phone_number.empty()) {
            if (!isNumber(phone_number)) {
                throw Err503();
            }
            this->phone_number = phone_number;
        }
        if (!address.empty()) {
            this->address = address;
        }
    }

    void editInformation(std::string password) {
        if (!password.empty()) {
            if (!isPasswordValid(password))
                throw Err503();

            this->password = password;
        }
    }
};

struct UserArray {
    int next_id = 0;
    std::vector<User> users;

    void addUser(User user) {
        user.id = next_id;
        users.push_back(user);
        next_id++;
    }

    User* getUser(std::string username) {
        for (auto& user : users) {
            if (user.username == username) {
                return &user;
            }
        }
        throw Err401();
    }

    json getUsersJson() {
        json users_json;
        for (auto& user : users) {
            json user_json;
            user_json["id"] = user.id;
            user_json["user"] = user.username;
            user_json["password"] = user.password;
            if (user.type == User::Type::ordinary) {
                user_json["admin"] = false;
                user_json["purse"] = user.purse;
                user_json["phoneNumber"] = user.phone_number;
                user_json["address"] = user.address;
            }
            else {
                user_json["admin"] = true;
            }
            users_json.push_back(user_json);
        }
        return users_json;
    }
};

struct Reservation {
    Reservation(int user_id, int num_of_beds, date::sys_days check_in_date, date::sys_days check_out_date) {
        this->user_id = user_id;
        this->num_of_beds = num_of_beds;
        this->check_in_date = check_in_date;
        this->check_out_date = check_out_date;
    }
    Reservation(json reservation_json) {
        this->user_id = reservation_json["id"];
        this->num_of_beds = reservation_json["numOfBeds"];
        parse(reservation_json["checkInDate"], this->check_in_date);
        parse(reservation_json["checkOutDate"], this->check_out_date);
    }

    int user_id;
    int num_of_beds;
    date::sys_days check_in_date;
    date::sys_days check_out_date;
};

struct Room {
    Room(int number, bool status, int price, int max_capacity) : number(number), status(status), price(price), max_capacity(max_capacity) {}
    Room(json room_json) {
        number = room_json["number"];
        status = room_json["status"];
        price = room_json["price"];
        max_capacity = room_json["maxCapacity"];
        for (auto& reservation : room_json["users"]) {
            Reservation new_reservation(reservation);
            reservations.push_back(new_reservation);
        }
    }
    int number;
    bool status;
    int price;
    int max_capacity;
    std::vector<Reservation> reservations;

    int getCapacity(date::sys_days cur_time) {
        int capacity = max_capacity;
        for (auto reservation : reservations) {
            if (reservation.check_in_date <= cur_time && cur_time < reservation.check_out_date) {
                capacity -= reservation.num_of_beds;
            }
        }
        return capacity;
    }

    void addReservation(Reservation reservation) {
        reservations.push_back(reservation);
    }

    bool isAvailable(int num_of_beds, date::sys_days check_in_date, date::sys_days check_out_date) {
        int capacity = max_capacity;
        for (auto reservation : reservations) {
            if (reservation.check_in_date <= check_in_date && check_in_date < reservation.check_out_date ||
                reservation.check_in_date < check_out_date && check_out_date <= reservation.check_out_date ||
                check_in_date <= reservation.check_in_date && reservation.check_out_date <= check_out_date) {
                capacity -= reservation.num_of_beds;
            }
        }
        return (capacity >= num_of_beds) ? true : false;
    }

    void removeReservation(int user_id, int count, date::sys_days cur_date) {
        for (int i = 0; i < reservations.size(); i++) {
            if (reservations[i].user_id == user_id) {
                if (cur_date >= reservations[i].check_in_date) {
                    throw Err102();
                }
                if (count > reservations[i].num_of_beds) {
                    throw Err102();
                }
                if (count == reservations[i].num_of_beds) {
                    reservations.erase(reservations.begin() + i);
                    return;
                }
                reservations[i].num_of_beds -= count;
                return;
            }
        }
        throw Err102();
    }

    void updateReservations(date::sys_days cur_date) {
        for (int i = 0; i < reservations.size(); i++) {
            if (reservations[i].check_out_date <= cur_date) {
                reservations.erase(reservations.begin() + i);
            }
        }
    }

    void leaveRoom(User* user, date::sys_days cur_date) {
        if (user->type == User::Type::admin) {
            for (int i = 0; i < reservations.size(); i++) {
                if (cur_date < reservations[i].check_in_date)
                    continue;
                reservations.erase(reservations.begin() + i);
            }
            return;
        }
        else {
            for (int i = 0; i < reservations.size(); i++) {
                if (reservations[i].user_id == user->id) {
                    if (cur_date < reservations[i].check_in_date)
                        throw Err102();
                    reservations.erase(reservations.begin() + i);
                    return;
                }
            }
            throw Err102();
        }
    }
};

struct RoomArray {
    std::vector<Room> rooms;

    void addRoom(Room room) {
        rooms.push_back(room);
    }

    void addRoom(int room_num, int max_capacity, int price) {
        for (auto room : rooms) {
            if (room.number == room_num) {
                throw Err111();
            }
        }
        Room new_room(room_num, false, price, max_capacity);
        rooms.push_back(new_room);
    }

    void modifyRoom(int room_num, int max_capacity, int price) {
        for (auto room : rooms) {
            if (room.number == room_num) {
                room.max_capacity = max_capacity;
                room.price = price;
                return;
            }
        }
        throw Err401();
    }

    void removeRoom(int room_num) {
        for (int i = 0; i < rooms.size(); i++) {
            if (rooms[i].number == room_num) {
                rooms.erase(rooms.begin() + i);
                return;
            }
        }
        throw Err401();
    }

    Room* getRoom(int room_num) {
        for (auto& room : rooms) {
            if (room.number == room_num) {
                return &room;
            }
        }
        throw Err101();
    }

    json getRoomsJson(date::sys_days cur_date) {
        json rooms_json;
        for (auto room : rooms) {
            json room_json;
            room_json["number"] = room.number;
            room_json["status"] = room.status;
            room_json["price"] = room.price;
            room_json["maxCapacity"] = room.max_capacity;
            room_json["capacity"] = room.getCapacity(cur_date);
            rooms_json.push_back(room_json);

            json reservations_json;
            for (auto reservation : room.reservations) {
                json reservation_json;
                reservation_json["id"] = reservation.user_id;
                reservation_json["numOfBeds"] = reservation.num_of_beds;
                reservation_json["checkInDate"] = to_string(reservation.check_in_date);
                reservation_json["checkOutDate"] = to_string(reservation.check_out_date);
                reservations_json.push_back(reservation_json);
            }
        }
        return rooms_json;
    }
};

#endif