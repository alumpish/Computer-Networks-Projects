#ifndef SERVER_TIMER_HPP
#define SERVER_TIMER_HPP

#include <pthread.h>

#include <mutex>
#include <string>
#include <vector>

#include "timer.hpp"

class ServerTimer {
public:
    ServerTimer(const std::string& config_file, Timer& timer);

    void addDays(int days);
    date::sys_days getCurrentDate() const;
    void setTime(const std::string& date);

    ~ServerTimer();

private:
    Timer& timer_;
    int server_fd_;

    std::vector<int> clients_fds_;
    std::mutex clients_list_lock_;

    pthread_t server_runner_id_;

    void sendCurrentTime(int fd);
    void publish();

    void handleIncomingSubscribers();
};

#endif