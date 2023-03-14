#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>
#include <vector>

struct User {
    User(int id, std::string username, std::string password, bool is_admin, int purse, std::string phone_number, std::string address) : id(id), username(username), password(password), is_admin(is_admin), purse(purse), phone_number(phone_number), address(address) {}
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
    std::vector<User> users;
    void addUser(User user) {
        users.push_back(user);
    }
};

struct Reservation {
    Reservation(int id, int num_of_beds) : id(id), num_of_beds(num_of_beds) {}
    Reservation(json reservation_json) {
        id = reservation_json["id"];
        num_of_beds = reservation_json["numOfBeds"];
        // reserve_date = reservation_json["reserve_date"];
        // check_out_date = reservation_json["check_out_date"];
    }
    int id;
    int num_of_beds;
    // std::string reserve_date;
    // std::string check_out_date;
};

struct Room {
    Room(int number, bool status, int price, int max_capacity, int capacity) : number(number), status(status), price(price), max_capacity(max_capacity), capacity(capacity) {}
    Room(json room_json) {
        number = room_json["number"];
        status = room_json["status"];
        price = room_json["price"];
        max_capacity = room_json["maxCapacity"];
        capacity = room_json["capacity"];
        for (auto reservation : room_json["users"]) {
            Reservation new_reservation(reservation);
            reservations.push_back(new_reservation);
        }
    }
    int number;
    bool status;
    int price;
    int max_capacity;
    int capacity;
    std::vector<Reservation> reservations;
    void addReservation(Reservation reservation) {
        reservations.push_back(reservation);
    }
};

struct RoomArray {
    std::vector<Room> rooms;
    void addRoom(Room room) {
        rooms.push_back(room);
    }
};

#endif