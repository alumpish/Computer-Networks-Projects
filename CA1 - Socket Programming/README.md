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

