#include "consts.hpp"
#include "hotel.hpp"
#include "server.hpp"
#include "server_handlers.hpp"
#include "timer.hpp"

void setupServerHandlers(Server& server, Hotel* hotel) {
    using namespace Consts::Paths;
    server.addHandler(new SignupUsernameHandler(hotel), SIGNUP_USERNAME);
    server.addHandler(new SignupUserInfoHandler(hotel), SIGNUP_INFO);
    server.addHandler(new SigninHandler(hotel), SIGNIN);
    server.addHandler(new ViewUserInfoHandler(hotel), VIEW_USER_INFO);
    server.addHandler(new ViewAllUsersHandler(hotel), VIEW_ALL_USERS);
    server.addHandler(new ViewRoomsInfoHandler(hotel), VIEW_ROOMS_INFO);
    server.addHandler(new BookingHandler(hotel), BOOKING);
    server.addHandler(new ViewReservationsHandler(hotel), VIEW_RESERVATIONS);
    server.addHandler(new CancelRoomHandler(hotel), CANCEL_ROOM);
    server.addHandler(new PassDayHandler(hotel), PASS_DAY);
    server.addHandler(new AdminEditInformationHandler(hotel), ADMIN_EDIT_INFO);
    server.addHandler(new EditInformationHandler(hotel), EDIT_INFO);
    server.addHandler(new AdminLeavingRoomHandler(hotel), ADMIN_LEAVING_ROOM);
    server.addHandler(new LeavingRoomHandler(hotel), LEAVING_ROOM);
    server.addHandler(new AddRoomHandler(hotel), ADD_ROOM);
    server.addHandler(new ModifyRoomHandler(hotel), MODIFY_ROOM);
    server.addHandler(new RemoveRoomHandler(hotel), REMOVE_ROOM);
    server.addHandler(new LogoutHandler(hotel), LOGOUT);
    server.addHandler(new GetUserTypeHandler(hotel), USER_TYPE);
}

int main() {
    Timer timer;
    Server server("./config.json", timer);
    Hotel* hotel = new Hotel(timer);
    setupServerHandlers(server, hotel);
    server.run();
}