# Computer Networks Course Project 1

## Server Side

### `Server`

The `Server` entity provides an interface to separate the back-end logic from connection handling. This separation is done through the method `void addHandler(RequestHandler* handler)`. (see [`RequestHandler`](#requesthandler)) The `RequestHandler` will provide a callback function that'll receive a request, process the request and return a response.

### `RequestHandler`

This is an abstract class that should only be used through inheriting other handlers from it. The abstraction is achieved by the pure virtual method `callback`; This method receives the request, processes it and returns a response.
