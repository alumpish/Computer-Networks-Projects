#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "command_handler.hpp"

class Client {
public:
    Client();
    void run();

private:
    CommandHandler cmd_handler_;

    void viewUserInformation(const std::vector<std::string>& input_args);
    void viewAllUsers(const std::vector<std::string>& input_args);
    void viewRoomsInformation(const std::vector<std::string>& input_args);
    void booking(const std::vector<std::string>& input_args);
    void canceling(const std::vector<std::string>& input_args);
    void passDay(const std::vector<std::string>& input_args);
    void editInformation(const std::vector<std::string>& input_args);
    void leavingRoom(const std::vector<std::string>& input_args);
    void rooms(const std::vector<std::string>& input_args);
    void logout(const std::vector<std::string>& input_args);
};

#endif