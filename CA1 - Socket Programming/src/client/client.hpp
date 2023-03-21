#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

#include "client_connector.hpp"
#include "command_handler.hpp"
#include "logger.hpp"
#include "response.hpp"
#include "timer.hpp"

class Client {
public:
    Client(Timer& timer);
    void run();

private:
    CommandHandler cas_cmd_handler_;
    CommandHandler cmd_handler_;
    Connector connector_;
    std::string session_id_;

    Logger logger_;
    std::ofstream* log_file_;
    Timer& timer_;
    std::string username_;

    struct {
        bool is_terminated = false;
        bool is_logged_out = true;
        bool authentication_finished = false;
    } cmd_flags_;

    enum class UserType {
        admin,
        ordinary
    } user_type_;

    void signIn(const std::vector<std::string>& input_args);
    void signupUsername(const std::vector<std::string>& input_args);
    void signupUserInfo(const std::vector<std::string>& input_args);

    void authenticate();

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

    void terminate(const std::vector<std::string>& input_args);

    void sendRequest(const std::string& path, const std::string& body);
    Response rcvResponse();

    void setUserType();
    void setupCASCmds();
    void setupAdminCmds();
    void setupOrdinaryUserCmds();
    void resetConnection();

    void runCmds();
    void dummyCommandNode(const std::vector<std::string>& input_args);
};

#endif