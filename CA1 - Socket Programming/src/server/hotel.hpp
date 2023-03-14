#ifndef HOTEL_HPP
#define HOTEL_HPP

#include "../common/types.hpp"
#include "json.hpp"


class Hotel {
    public:
    void readUsers();
    void readRooms();

    private:
    UserArray users_;
    RoomArray rooms_;

}

#endif