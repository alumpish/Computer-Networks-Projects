#ifndef SERVER_HANDLERS_HPP
#define SERVER_HANDLERS_HPP

#include "hotel.hpp"
#include "request.hpp"
#include "request_handler.hpp"
#include "response.hpp"

class HotelHandler : public RequestHandler {
public:
    HotelHandler(Hotel* hotel)
        : hotel_(hotel) {}

protected:
    Hotel* hotel_;
};

class SignupUsernameHandler : public HotelHandler {
public:
    SignupUsernameHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class SignupUserInfoHandler : public HotelHandler {
public:
    SignupUserInfoHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class SigninHandler : public HotelHandler {
public:
    SigninHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class ViewUserInfoHandler : public HotelHandler {
public:
    ViewUserInfoHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class ViewAllUsersHandler : public HotelHandler {
public:
    ViewAllUsersHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class ViewRoomsInfoHandler : public HotelHandler {
public:
    ViewRoomsInfoHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class BookingHandler : public HotelHandler {
public:
    BookingHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class ViewReservationsHandler : public HotelHandler {
public:
    ViewReservationsHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class CancelRoomHandler : public HotelHandler {
public:
    CancelRoomHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class PassDayHandler : public HotelHandler {
public:
    PassDayHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class AdminEditInformationHandler : public HotelHandler {
public:
    AdminEditInformationHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class EditInformationHandler : public HotelHandler {
public:
    EditInformationHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class AdminLeavingRoomHandler : public HotelHandler {
public:
    AdminLeavingRoomHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class LeavingRoomHandler : public HotelHandler {
public:
    LeavingRoomHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class AddRoomHandler : public HotelHandler {
public:
    AddRoomHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class ModifyRoomHandler : public HotelHandler {
public:
    ModifyRoomHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class RemoveRoomHandler : public HotelHandler {
public:
    RemoveRoomHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class LogoutHandler : public HotelHandler {
public:
    LogoutHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

class GetUserTypeHandler : public HotelHandler {
public:
    GetUserTypeHandler(Hotel* hotel)
        : HotelHandler(hotel) {}

protected:
    Response handleResponse(const Request& request) override;
};

#endif