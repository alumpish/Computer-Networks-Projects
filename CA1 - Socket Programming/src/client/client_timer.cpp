#include "client_timer.hpp"

#include <pthread.h>
#include <sys/select.h>
#include <unistd.h>

#include <string>
#include <thread>

#include "net.hpp"
#include "timer.hpp"

ClientTimer::ClientTimer(Timer& timer)
    : timer_(timer) {
    socket_fd_ = net::connectTCPClient("127.0.0.1", 8001);

    std::thread runner(&ClientTimer::handleIncomingUpdates, this);
    runner_id_ = runner.native_handle();
    runner.detach();
}

date::sys_days ClientTimer::getCurrentDate() const {
    return timer_.getCurrentDate();
}

ClientTimer::~ClientTimer() {
    pthread_cancel(runner_id_);
    close(socket_fd_);
}

void ClientTimer::handleIncomingUpdates() {
    fd_set master_set;
    FD_SET(socket_fd_, &master_set);
    while (true) {
        fd_set working_set = master_set;
        select(socket_fd_ + 1, &working_set, nullptr, nullptr, nullptr);
        std::string current_time = net::rcvMessage(socket_fd_);
        timer_.setTime(current_time);
    }
}
