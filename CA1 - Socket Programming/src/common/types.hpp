#ifndef TYPES_HPP
#define TYPES_HPP

#include <date/date.h>

#include <string>
#include <vector>

struct User {
    User(std::string username, std::string password, bool is_admin, int purse, std::string phone_number, std::string address) : username(username), password(password), is_admin(is_admin), purse(purse), phone_number(phone_number), address(address) {}
    User(json user_json) {
        id = user_json["id"];
        username = user_json["user"];
        password = user_json["password"];
        is_admin = user_json["admin"];
        purse = user_json["purse"];
        phone_number = user_json["phoneNumber"];
        address = user_json["address"];
    }

    int id;
    std::string username;
    std::string password;
    bool is_admin;
    int purse;
    std::string phone_number;
    std::string address;
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
        for (auto user : users) {
            if (user.username == username) {
                return &user;
            }
        }
        throw BadRequest();
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
        this->check_in_date = date::parse("%Y-%m-%d", reservation_json["checkInDate"]);
        this->check_out_date = date::parse("%Y-%m-%d", reservation_json["checkOutDate"]);
    }

    int user_id;
    int num_of_beds;
    date::sys_days check_in_date;
    date::sys_days check_out_date;
};

struct Room {
    Room(int number, bool status, int price, int max_capacity, int capacity) : number(number), status(status), price(price), max_capacity(max_capacity), capacity(capacity) {}
    Room(json room_json) {
        number = room_json["number"];
        status = room_json["status"];
        price = room_json["price"];
        max_capacity = room_json["maxCapacity"];
        for (auto reservation : room_json["users"]) {
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
        return (capacity >= num_of_beds) : true ? false;
    }

    void removeReservation(int user_id, int count) {
        for (int i = 0; i < reservations.size(); i++) {
            if (reservations[i].user_id == user_id) {
                if (count > reservations[i].num_of_beds) {
                    throw BadRequest();
                }
                if (count == reservations[i].num_of_beds) {
                    reservations.erase(reservations.begin() + i);
                    return;
                }
                reservations[i].num_of_beds -= count;
                return;
            }
        }
        throw BadRequest();
    }
};

struct RoomArray {
    std::vector<Room> rooms;
    void addRoom(Room room) {
        rooms.push_back(room);
    }
    Room getRoom(int room_num) {
        for (auto room : rooms) {
            if (room.number == room_num) {
                return room;
            }
        }
        throw BadRequest();
    }
};

#endif