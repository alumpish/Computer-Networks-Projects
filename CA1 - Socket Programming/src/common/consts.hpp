#ifndef CONSTS_HPP
#define CONSTS_HPP

#include <string>

namespace Consts {

namespace Timer {
    const std::string DATE_FORMAT = "(\\d\\d)-(\\d\\d)-(\\d\\d\\d\\d)";
}

namespace Paths {
    const std::string SIGNUP_USERNAME = "/signup/username";
    const std::string SIGNUP_INFO = "/signup/info";
    const std::string SIGNIN = "/signin";
    const std::string VIEW_USER_INFO = "/user_info";
    const std::string VIEW_ALL_USERS = "/all_users";
    const std::string VIEW_ROOMS_INFO = "/rooms_info";
    const std::string BOOKING = "/booking";
    const std::string VIEW_RESERVATIONS = "/view_reservations";
    const std::string CANCEL_ROOM = "/cancel_room";
    const std::string PASS_DAY = "/pass_day";
    const std::string ADMIN_EDIT_INFO = "/admin_edit_info";
    const std::string EDIT_INFO = "/edit_info";
    const std::string ADMIN_LEAVING_ROOM = "/admin_leaving_room";
    const std::string LEAVING_ROOM = "/leaving_room";
    const std::string ADD_ROOM = "/add_room";
    const std::string MODIFY_ROOM = "/modify_room";
    const std::string REMOVE_ROOM = "/remove_room";
    const std::string LOGOUT = "/logout";
    const std::string USER_TYPE = "/user_type";
} // namespace Paths

} // namespace Consts
#endif