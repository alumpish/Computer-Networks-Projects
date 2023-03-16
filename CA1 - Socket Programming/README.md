# Computer Networks Course Project 1

## Saman Eslami: 810199375

## Ali Padyav: 810199388

<br/>
<br />

## Server Side

### `Server`

The `Server` entity provides an interface to separate the back-end logic from connection handling. This separation is done through the method `void addHandler(RequestHandler* handler)`. (see [`RequestHandler`](#requesthandler)) The `RequestHandler` will provide a callback function that'll receive a request, process the request and return a response.

### `RequestHandler`

This is an abstract class that should only be used through inheriting other handlers from it. The abstraction is achieved by the pure virtual method `callback`; This method receives the request, processes it and returns a response.

### `Important Types`

`User`

The struct has several member variables, including an integer ID, a username string, a password string, a boolean indicating whether the user is an administrator, an integer for the user's purse (perhaps representing their balance or account funds), a phone number string, and an address string.

The struct also has two constructors, one that takes in several arguments including the username, password, is_admin, purse, phone_number, and address, and another that takes in a JSON object representing the user and creates a User object from that JSON.

```cpp
struct User {
    User(std::string username, std::string password, bool is_admin, int purse, std::string phone_number, std::string address);
    User(json user_json);

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
}
```

### Important Methods

- editInformation : This function is overloaded for ordinary and admin user.

```cpp
void editInformation(std::string password, std::string phone_number, std::string address);

void editInformation(std::string password);
```

`Reservation`

Reservation struct represents a user reservation for a certain number of beds during a specific time period, specified by check_in_date and check_out_date. It has two constructors, one taking the reservation details as individual parameters and the other taking a JSON object as input. The struct has four member variables: user_id, num_of_beds, check_in_date, and check_out_date

```cpp
struct Reservation {
    Reservation(int user_id, int num_of_beds, date::sys_days check_in_date, date::sys_days check_out_date);
    Reservation(json reservation_json);

    int user_id;
    int num_of_beds;
    date::sys_days check_in_date;
    date::sys_days check_out_date;
};
```

- `Room`

struct called Room that represents a hotel room with member variables for number, status, price, max_capacity, and a vector reservations. The struct has two constructors, one that takes four arguments and initializes member variables and another that takes a json object and extracts its values to initialize member variables and reservations vector.

```cpp
struct Room {
   Room(int number, bool status, int price, int max_capacity);
   Room(json room_json);
   int number;
   bool status;
   int price;
   int max_capacity;
   std::vector<Reservation> reservations;
}
```

### Important Methods

- isAvailable : Check if there are sufficient bed in given room and date.

```cpp
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
```

- updateReservations : Remove reservations that their out date is passed.

```cpp
void updateReservations(date::sys_days cur_date) {
    for (int i = 0; i < reservations.size(); i++) {
        if (reservations[i].check_out_date <= cur_date) {
            reservations.erase(reservations.begin() + i);
        }
    }
}
```

- leaveRoom : This method is used to remove a current reservation from a room by admin or ordinary user.

```cpp
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
```

<br/>
<br/>

### `Hotel Class`

The Hotel class represents a hotel system with various functionalities such as managing user accounts, booking and canceling rooms, and managing room information.

`Public Methods`

```cpp
Hotel(const std::string& cur_date)
```

A constructor that initializes the hotel system with the current date.

```cpp
void readUsers()
```

Reads user information from a file.

```cpp
void readRooms()
```

Reads room information from a file.

```cpp
bool isUserExist(const std::string& username)
```

Returns a boolean indicating whether a user with the given username exists in the system.

```cpp
void addSession(const std::string& session_id, const std::string& username)
```

Adds a new session to the system with the given session ID and username.

```cpp
void removeSession(const std::string& session_id)
```

Removes a session with the given session ID from the system.

```cpp
void addUser(User user)
```

Adds a new user to the system.

```cpp
void signIn(const std::string& session_id, const std::string& username, const std::string& password)
```

Authenticates a user by checking if the given username and password match the user's credentials in the system.

```cpp
std::string getUsername(const std::string& session_id)
```

Returns the username associated with the given session ID.

```cpp
json getUserInfo(const std::string& session_id)
```

Returns a JSON object containing the information of the user associated with the given session ID.

```cpp
json getAllUsersInfo(const std::string& session_id)
```

Returns a JSON array containing information of all the users in the system.

```cpp
json getAllRoomsInfo(const std::string& session_id, date::sys_days cur_time)
```

Returns a JSON array containing information of all the rooms in the system with their availability status.

```cpp
void bookRoom(const std::string& username, int room_num, int num_of_beds, date::sys_days check_in_date, date::sys_days check_out_date)
```

Books a room for the given user with the specified room number, number of beds, check-in date, and check-out date.

```cpp
json getReservations(const std::string& username)
```

Returns a JSON array containing information about all the reservations made by the given user.

```cpp
void cancelReservation(const std::string& username, int room_num, int count)
```

Cancels the reservation made by the given user for the specified room and the number of beds.

```cpp
void editInformation(const std::string& session_id, const std::string& password, const std::string& phone_number, const std::string& address)
```

Edits the information of the user associated with the given session ID.

```cpp
void editInformation(const std::string& session_id, const std::string& password)
```

Edits the password of the user associated with the given session ID.

```cpp
void passDay(const std::string& session_id, int days)
```

Passes the specified number of days and updates the room availability status accordingly.

```cpp
void updateRooms(date::sys_days cur_date)
```

Updates the room availability status based on the current date.

```cpp
void leaveRoom(const std::string& session_id, int room_num)
```

Releases the specified room by the user associated with the given session ID.

```cpp
void addRoom(const std::string& session_id, int room_num, int max_capacity, int price)
```

Adds a new room with the specified room number, maximum capacity, and price.

```cpp
void modifyRoom(const std::string& session_id, int room_num, int max_capacity, int price)
```

Modifies the information of the room with the specified room number.

<br/>

`Private`

```cpp
private:
    Timer timer_;
    UserArray users_;
    RoomArray rooms_;
    std::unordered_map<std::string, std::string> sessions_un_map_;
```

- `timer_` : A private member variable of type Timer used to keep track of time.

- `users_` : A private member variable of type UserArray, which is an array of User objects. This array is used to store information about registered users.

- `rooms_` : A private member variable of type RoomArray, which is an array of Room objects. This array is used to store information about the available rooms in the hotel.

- `sessions_un_map_` : A private member variable of type unordered_map. This map is used to store the session IDs and the corresponding usernames of the users who have signed in to the hotel system.
