#include "request_handler.hpp"

#include "exceptions.hpp"
#include "request.hpp"
#include "response.hpp"

Response RequestHandler::callback(const Request& request) {
    Response response;
    try {
        response = handleResponse(request);
    }
    catch (const CustomException& e) {
        response = Response(e);
    }
    response.setSessionID(request.getSessionID());
    return response;
}
