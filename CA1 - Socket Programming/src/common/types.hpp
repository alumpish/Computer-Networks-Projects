#ifndef TYPES_HPP
#define TYPES_HPP

#include <sstream>
#include <string>
#include <vector>

#include "date.h"
#include "exceptions.hpp"
#include "json.hpp"
#include "timer.hpp"

using json = nlohmann::json;

struct User {
    User(
        std::string username,
        std::string password,
        bool is_admin,
        int purse,
        std::string phone_number,
        std::string address
    ) {
        this->username = username;
        this->password = password;
        if (is_admin) {
            type = User::Type::admin;
            return;
        }
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

    std::string toString() const {
        std::ostringstream os;
        os << "id: " << id << std::endl;
        os << "username: " << username << std::endl;
        os << "password: " << password << std::endl;
        if (type == Type::admin)
            return os.str();
        os << "purse: " << purse << std::endl;
        os << "phone_number: " << phone_number << std::endl;
        os << "address: " << address << std::endl;
        return os.str();
    }

    json toJSON() const {
        json result;
        result["id"] = id;
        result["user"] = username;
        result["password"] = password;
        if (type == Type::admin) {
            result["admin"] = true;
            return result;
        }
        result["admin"] = false;
        result["purse"] = purse;
        result["phoneNumber"] = phone_number;
        result["address"] = address;
        return result;
    }

    json safeViewToJSON() const {
        json result;
        result["id"] = id;
        result["user"] = username;
        if (type == Type::admin) {
            result["admin"] = true;
            return result;
        }
        result["admin"] = false;
        result["purse"] = purse;
        result["phoneNumber"] = phone_number;
        result["address"] = address;
        return result;
    }

    static std::string jsonSafeViewToString(const json& user_json) {
        std::ostringstream os;
        os << "id: " << user_json["id"] << std::endl;
        os << "username: " << user_json["user"] << std::endl;
        if (user_json["admin"])
            return os.str();
        os << "purse: " << user_json["purse"] << std::endl;
        os << "phone_number: " << user_json["phoneNumber"] << std::endl;
        os << "address: " << user_json["address"] << std::endl;
        return os.str();
    }

    static std::string jsonToString(const json& user_json) {
        User user(user_json);
        return user.toString();
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
    std::vector<User*> users;

    void addUser(User* user) {
        user->id = next_id;
        users.push_back(user);
        next_id++;
    }

    User* getUser(std::string username) {
        for (auto& user : users) {
            if (user->username == username) {
                return user;
            }
        }
        throw Err401();
    }

    json getUsersJson() const {
        json users_json;
        for (const auto& user : users) {
            users_json.push_back(user->toJSON());
        }
        json final;
        final["users"] = users_json;
        return final;
    }

    json getUsersSafeJSON() const {
        json result;
        for (const auto& user : users) {
            result.push_back(user->safeViewToJSON());
        }
        return result;
    }
};

struct Reservation {
    Reservation(
        int user_id,
        int num_of_beds,
        date::sys_days check_in_date,
        date::sys_days check_out_date
    ) {
        this->user_id = user_id;
        this->num_of_beds = num_of_beds;
        this->check_in_date = check_in_date;
        this->check_out_date = check_out_date;
    }
    Reservation(json reservation_json) {
        this->user_id = reservation_json["id"];
        this->num_of_beds = reservation_json["numOfBeds"];
        check_in_date = Timer::parseString(reservation_json["checkInDate"]);
        check_out_date = Timer::parseString(reservation_json["checkOutDate"]);
    }

    std::string toString() const {
        std::ostringstream os;
        os << "  user_id: " << user_id << std::endl;
        os << "  num_of_beds: " << num_of_beds << std::endl;
        os << "  check_in_date: " << date::format("%d-%m-%Y", check_in_date) << std::endl;
        os << "  check_out_date: " << date::format("%d-%m-%Y", check_out_date) << std::endl;
        return os.str();
    }

    int user_id;
    int num_of_beds;
    date::sys_days check_in_date;
    date::sys_days check_out_date;
};

struct Room {
    Room(
        int number, bool status, int price, int max_capacity
    ) : number(number),
        status(status),
        price(price),
        max_capacity(max_capacity) {}

    Room(json room_json) {
        number = room_json["number"];
        status = room_json["status"];
        price = room_json["price"];
        max_capacity = room_json["maxCapacity"];
        for (auto& reservation : room_json["reservations"]) {
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

    std::string toString() const {
        std::ostringstream os;
        os << "number: " << number << std::endl;
        os << "status: " << status << std::endl;
        os << "price: " << price << std::endl;
        os << "max_capacity: " << max_capacity << std::endl;
        os << "reservations: " << std::endl;
        for (auto reservation : reservations) {
            os << reservation.toString();
        }
        return os.str();
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
        for (auto& room : rooms) {
            if (room.number == room_num) {
                room.max_capacity = max_capacity;
                room.price = price;
                return;
            }
        }
        throw Err401();
    }

    void removeRoom(int room_num, date::sys_days cur_date) {
        for (int i = 0; i < rooms.size(); i++) {
            if (rooms[i].number == room_num) {
                if (rooms[i].getCapacity(cur_date) == 0)
                    throw Err109();
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

            json reservations_json;
            for (auto reservation : room.reservations) {
                json reservation_json;
                reservation_json["id"] = reservation.user_id;
                reservation_json["numOfBeds"] = reservation.num_of_beds;
                reservation_json["checkInDate"] = Timer::dateToString(reservation.check_in_date);
                reservation_json["checkOutDate"] = Timer::dateToString(reservation.check_out_date);
                reservations_json.push_back(reservation_json);
            }
            room_json["reservations"] = reservations_json;
            rooms_json.push_back(room_json);
        }
        json final;
        final["rooms"] = rooms_json;
        return final;
    }
};

#endif