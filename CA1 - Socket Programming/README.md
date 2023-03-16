# Computer Networks Course Project 1

## Saman Eslami: 810199375

## Ali Padyav: 810199388

<br/>
<br />

# Server Side

## `Server`

The `Server` entity provides an interface to separate the back-end logic from connection handling. This separation is done through the method `void addHandler(RequestHandler* handler)`. (see [`RequestHandler`](#requesthandler)) The `RequestHandler` will provide a callback function that'll receive a request, process the request and return a response.

<br/>

## `RequestHandler`

This is an abstract class that should only be used through inheriting other handlers from it. The abstraction is achieved by the pure virtual method `callback`; This method receives the request, processes it and returns a response.

<br/>

## `ServerConnector Class`

The `ServerConnector` class is responsible for connecting to the server and sending and receiving data. It has a `connect` method that takes the server's IP address and port number as arguments and connects to the server. It also has a `send` method that takes a string as an argument and sends it to the server. The `receive` method receives data from the server and returns it as a string.

Main method of this class is `pollForEvent` that is responsible for receiving data from client using select system call.

```cpp
Connector::Event Connector::pollForEvent() {
    fd_set working_set = fds_.master_fds_set;
    select(fds_.max_fd + 1, &working_set, nullptr, nullptr, nullptr);
    if (FD_ISSET(STDIN_FILENO, &working_set))
        return {Connector::Event::EventType::stdin_cmd, STDIN_FILENO};
    if (FD_ISSET(fds_.server_fd, &working_set))
        return {Connector::Event::EventType::incoming_client, fds_.server_fd};
    for (const auto& cur_client_fd : fds_.client_fds)
        if (FD_ISSET(cur_client_fd, &working_set))
            return {Connector::Event::EventType::client_req, cur_client_fd};
}
```

<br/>

## `Structs`

Most of logic is implemented in structs. There are 5 main structs:

### `1.User`

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

`Important Methods`

- editInformation : This function is overloaded for ordinary and admin user.

```cpp
void editInformation(std::string password, std::string phone_number, std::string address);

void editInformation(std::string password);
```

### `2.UserArray`

The `UserArray` struct contains a vector of `User` objects and provides methods to add and get users from the vector.

### Properties

- `next_id`: An integer representing the ID that will be assigned to the next `User` object added to the vector.

### Methods

- `addUser(User user)`: Adds a `User` object to the vector, assigning it the next available ID.
- `getUser(std::string username)`: Returns a pointer to the `User` object with the specified username. Throws an `Err401` exception if the user does not exist in the vector.

Note: `Err401` is a custom exception class that is not defined in this code snippet.

### `3.Reservation`

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

### `4.Room`

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

### `5.RoomArray`

The `RoomArray` struct contains a vector of `Room` objects and provides methods to add, modify, remove, and get rooms from the vector.

### Methods

- `addRoom(Room room)`: Adds a `Room` object to the vector.
- `addRoom(int room_num, int max_capacity, int price)`: Adds a new `Room` object to the vector with the specified room number, maximum capacity, and price. Throws an `Err111` exception if a room with the same number already exists in the vector.
- `modifyRoom(int room_num, int max_capacity, int price)`: Modifies the maximum capacity and price of the `Room` object with the specified room number. Throws an `Err401` exception if the room does not exist in the vector.
- `removeRoom(int room_num)`: Removes the `Room` object with the specified room number from the vector. Throws an `Err401` exception if the room does not exist in the vector.
- `getRoom(int room_num)`: Returns a pointer to the `Room` object with the specified room number. Throws an `Err101` exception if the room does not exist in the vector.

Note: `Err101`, `Err111`, and `Err401` are custom exception classes that are not defined in this code snippet.

<br/>

## `Hotel Class`

The `Hotel` class is responsible for storing and managing the hotel data. It includes a number of private variables, including a `RoomArray` object (`rooms_`), a `UserArray` object (`users_`), and a `Timer` object (`timer_`). It also includes a number of private functions, each of which corresponds to a specific command that the server can receive from a client.

<br/>

## `Timer Class`

The `Timer` class is a simple class that represents a date and time. It has a public `constructor` that takes a `date::sys_days` object and initializes the `date_` member variable. It also has several public member functions that can be used to interact with the timer. These functions include:

`getCurrentDate()`: returns the current date.

`addDays(int days)`: advances the timer by the given number of days.

<br/>
<br/>

# **Client Side**

## `Client Class`

`Client` class is responsible for handling various commands and interacting with a server via a `Connector` class. The `Client` class includes a few private variables, including two instances of a `CommandHandler` class (`cas_cmd_handler_` and `cmd_handler_`), a Connector object (`connector_`), and a string for storing a session ID (`session_id_`).

The Client class also includes a number of private functions, each of which corresponds to a specific command that the client can handle. These functions include commands for user authentication (`signin`, `signupUsername`, `signupUserInfo`, `authenticate`), viewing user and room information (`viewUserInformation`, `viewAllUsers`, `viewRoomsInformation`), booking and cancelling rooms (`booking`, `canceling`, `cancelRoom`, `leavingRoom`), editing information (`editInformation`), adding/modifying/removing rooms (`addRoom`, `modifyRoom`, `removeRoom`), and logging out or terminating the client (`logout`, `terminate`).

<br/>

## `ClientConnector Class`

Connector class represents a network socket connection. The constructor initializes a socket file descriptor (`sock_fd_`) with the given port number and host name.

The `sendMessage` method sends a message to the connected socket by writing the provided message string to the socket file descriptor.

The `rcvMessage` method receives a message from the connected socket by reading from the socket file descriptor and returning the received message as a string.

```cpp
class Connector {
public:
    Connector(int port, const std::string& host_name);

    void sendMessage(const std::string& msg);
    std::string rcvMessage();

private:
    int sock_fd_;
};
```

<br/>

### And **`main`** code of Client is like this:

```cpp
int main() {
    Client client;
    client.run();
}
```

<br/>
<br/>

# **Both Sides**

The conncection between the client and the serrver is by Request-Response protocol. The client sends a request to the server and the server responds to the request. The client can send multiple requests to the server and the server can respond to multiple requests. The client and the server can send and receive messages in any order.

The client sends a request to the server by sending a string that contains the command name and the arguments of the command. The server responds to the request by sending a string that contains the result of the command execution.

<br/>

## `Request class`

The `Request` class is a simple class that represents a request and uses Json format. It has a public `constructor` that takes a `std::string` object and initializes the `request_` member variable. It also has several public member functions that can be used to interact with the request. These functions include:

`setPath()`: sets the path of the request.  
`setSessionID()`: sets the session_id of the request.
`setBody()`: sets the body of the request.

And get methods for each of the member variables.

`toJSON()`: returns the request as a JSON string.

<br/>

## `Response class`

It's structure is similar to the `Request` class.

<br/>

## `commandHandler class`

This class can handle different commands. The constructor initializes the `input_stream_` variable and creates two instances of `CommandNode` objects: `init_root_` and `root_`, which are initially pointing to the same object. The class also provides methods to add a new command, run a single command, reset the root, and delete all commands.

The `addCommand` method takes a command name and a pointer to a Command object and adds a new `CommandNode` to the tree structure with the provided name and Command pointer.

The `runSingleCommand` method reads a command name from the input stream, searches for the CommandNode in the tree structure with the same name as the input, and executes the command by calling `execCommand` with the appropriate arguments. If the command is not found or the arguments do not match the expected number or types, an exception is thrown.

The `resetRoot` method resets the `root_` variable to the initial root, effectively resetting the tree structure to its original state.

The `currentLevelCommandsToString` method returns a string with a list of all the command names and their help messages for the current level of the tree structure.

The `allLevelsCommandsToString` method is not yet implemented.

The `operator[]` method returns a new `CommandHandler` object that has the `root_` variable set to the `CommandNode` with the provided command name.

The `deleteCommand`s method deallocates all the `CommandNode `objects in the tree structure.

Finally, the class provides a `destructor` that deallocates the initial root object.
