#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include "client_connector.hpp"
#include "command_handler.hpp"

class Client {
public:
    Client();
    void run();

private:
    CommandHandler cmd_handler_;
    Connector connector_;
    std::string session_id_;

    enum class UserType {
        admin,
        ordinary
    } user_type_;

    void viewUserInformation(const std::vector<std::string>& input_args);

    void viewAllUsers(const std::vector<std::string>& input_args);

    void viewRoomsInformation(const std::vector<std::string>& input_args);

    void booking(const std::vector<std::string>& input_args);

    void canceling(const std::vector<std::string>& input_args);
    void cancelRoom(const std::vector<std::string>& input_args);

    void passDay(const std::vector<std::string>& input_args);

    void editInformation(const std::vector<std::string>& input_args);

    void leavingRoom(const std::vector<std::string>& input_args);

    void addRoom(const std::vector<std::string>& input_args);
    void modifyRoom(const std::vector<std::string>& input_args);
    void removeRoom(const std::vector<std::string>& input_args);

    void logout(const std::vector<std::string>& input_args);

    void sendRequest(const std::string& path, const std::string& body);
};

#endif