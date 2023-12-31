# CA1 - Socket Programming

## Contents

- [Server Side](#server_side)
- [Client Side](#client-side)
- [Both Sides](#both-sides)
- [How to run](#how-to-run)

# **Server Side**

## `Server`

The `Server` entity provides an interface to separate the back-end logic from connection handling. This separation is done through the method `void addHandler(RequestHandler* handler)`. (see [`RequestHandler`](#requesthandler)) The `RequestHandler` will provide a callback function that'll receive a request, process the request and return a response.

### `Server`'s construction

The constructor of server takes a config file address to read its initial information needed for the connection, and a `Timer&` to retrieve and change the system's time. The constructor's definition also contains logger's initialization and command setup. Here's a brief description of `Server`'s methods:

- `run`: This method waits for an event to occur. After that it checks what kind of event has occurred and decides which handler function to call.
- `addHandler`: This method maps a path to its handler.
- `handleIncomingClient`: This method checks for the incoming connections and accepts them if possible.
- `handleIncomingRequest`: This method received the incoming request, extracts its path and call the proper handler for it. The rest is handled inside the `RequestHandler`.
- `handleSTDINCommand`: This method simply calls the `CommandHandler`'s method to run a single command. Note that this method is called because there are some strings pending in stdin's buffer. Proper command handling method is called later by the `CommandHandler`.
- `genSessionID`: Generates a random string to be used as clients' sessions IDs.
- `findRequestHandler`: Search for the `RequestHandler` that is mapped to the given path.
- `isAuthorized`: Checks if the given session ID is present among the active sessions.
- `setupCommands`: Sets up the stdin commands. These commands are "setTime < date >" and "exit".
- `setTime`: This method is the handler for the stdin's command with the same name.
- `exit`: same as `setTime`.

<br/>

## Server's `Connector`

As the server's responsibilities grew, the class declaration started to get big; The `Connector`'s purpose is to wrap around server's responsibilities and provide tcp connection functionalities. Here's a brief description of `Connector`'s methods:

- `rcvMessage`: This method reads the given socket and return its message.
- `sendMessage`: This one writes the given message to the given socket.
- `acceptClient`: This method accepts an incoming client (if possible).
- `removeConnection`: This method removes the given client's connection. This is done in several steps; First it will remove the client's socket fd from the list. Then it will update the information needed for `select` (This includes the `max_fd` and the `master_fds_set`). In the end it'll close the socket and return to the caller.
- `pollForEvent`: This is actually a wrapper around the `select` method. It calls the `select` on the server + clients + stdin file descriptors and returns the event type along with the file descriptor related to the event.

Note that there is no method to detect if the connection is closed from the client's side. To detect this issue we must do `read` on the client's socket and see if it returns 0. This can't be done because we don't know that the socket is waiting to send EOF or is actually sending a message; So we will watch the `rcvMessage` and in case of any errors thrown, we will close the connection. This isn't the best way to do this, but it's not the worst one either.

### Methods

## `RequestHandler`

This is an abstract class that should only be used through inheriting other handlers from it. The abstraction is achieved by the pure virtual method `callback`; This method receives the request, processes it and returns a response.

## Server Handlers

These are the custom handlers that has overridden the proper methods from `RequestHandle`. I won't get into the details as they are obvious by their names.

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

### And **`main`** code of Client is like this

```cpp
int main() {
    Client client;
    client.run();
}
```

<br/>
<br/>

# **Both Sides**

The conncection between the client and the server is by Request-Response protocol. The client sends a request to the server and the server responds to the request. The client can send multiple requests to the server and the server can respond to multiple requests. The client and the server can send and receive messages in any order.

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

The `deleteCommand`s method deallocates all the `CommandNode`objects in the tree structure.

Finally, the class provides a `destructor` that deallocates the initial root object.

# **How to run**

In root directory:
```bash
make
```

<br/>

### **Server**

```bash
cd src/server && ./server.out
```
### then:
```bash
initTime <date: %d-%m-%Y>
```

<br/>

### **Client**

```bash
cd src/client && ./client.out
```
### then:
We can signin or signup
If signin is successful, we can see the main menu of the client.
Else we can see the error message.

![alt text](./images/scr%20(1).png)

<br/>
Command showUserInfo shows the information of the user.

![alt text](./images/scr%20(2).png)

<br/>
Command showAllUsers shows the information of all users.
if you are not admin, you can't see the information of other users and you can see the error message.

![alt text](./images/scr%20(3).png)

<br/>
You can modify your information by command editUserInfo.
If values of arguments are not valid, you can see the error message.

![alt text](./images/scr%20(7).png)

![alt text](./images/scr%20(5).png)

<br/>
You can go ahead in date by command pass_day.
If you are not admin, you can't pass the day and you can see the error message.

![alt text](./images/scr%20(6).png)

<br/>

### Rooms commands:

Add a room by command add_room.
Id room num is exist you can see the error message.

![alt text](./images/scr%20(8).png)

![alt text](./images/scr%20(9).png)

<br/>

Remove a room by command remove_room. If room num is not exist, you can see the error message.

![alt text](./images/scr%20(10).png)

![alt text](./images/scr%20(11).png)

<br/>

Leave a room by command room <room_num>.
If room num is not exist or you are not in this room, you can see the error message.

![alt text](./images/scr%20(12).png)

![alt text](./images/scr%20(13).png)

![alt text](./images/scr%20(14).png)

<br/>

### Show rooms info in ordinary user and admin.

Ordinary:

![alt text](./images/scr%20(15).png)

<br/>
Admin:


![alt text](./images/scr%20(16).png)

<br/>
