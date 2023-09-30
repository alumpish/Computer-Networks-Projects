#ifndef CLIENT_TIMER_HPP
#define CLIENT_TIMER_HPP

#include <pthread.h>

#include <string>

#include "timer.hpp"

class ClientTimer {
public:
    ClientTimer(Timer& timer);

    date::sys_days getCurrentDate() const;

    ~ClientTimer();

private:
    Timer& timer_;

    int socket_fd_;

    pthread_t runner_id_;

    void handleIncomingUpdates();
};

#endif