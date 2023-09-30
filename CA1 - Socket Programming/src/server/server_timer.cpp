#include "server_timer.hpp"

#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "exceptions.hpp"
#include "net.hpp"
#include "response.hpp"
#include "timer.hpp"

ServerTimer::ServerTimer(const std::string& config_file, Timer& timer)
    : timer_(timer) {
    std::ifstream f(config_file);
    json data = json::parse(f)["timer_server"];
    int port = data["port"];
    std::string host_name = data["hostName"];
    int max_clients = data["maxClients"];

    server_fd_ = net::connectTCPServer(host_name, port, max_clients);

    std::thread server_runner(&ServerTimer::handleIncomingSubscribers, this);
    server_runner_id_ = server_runner.native_handle();
    server_runner.detach();
}

void ServerTimer::addDays(int days) {
    timer_.addDays(days);
    publish();
}

date::sys_days ServerTimer::getCurrentDate() const {
    return timer_.getCurrentDate();
}

void ServerTimer::setTime(const std::string& date) {
    timer_.setTime(date);
    publish();
}

ServerTimer::~ServerTimer() {
    pthread_cancel(server_runner_id_);
    for (const auto& client : clients_fds_)
        close(client);
    close(server_fd_);
}

void ServerTimer::sendCurrentTime(int fd) {
    std::string current_time = Timer::dateToString(timer_.getCurrentDate());
    net::sendMessage(fd, current_time);
}

void ServerTimer::publish() {
    clients_list_lock_.lock();
    for (const auto& client : clients_fds_)
        sendCurrentTime(client);
    clients_list_lock_.unlock();
}

void ServerTimer::handleIncomingSubscribers() {
    fd_set master_set;
    FD_SET(server_fd_, &master_set);
    while (true) {
        fd_set working_set = master_set;
        select(server_fd_ + 1, &working_set, nullptr, nullptr, nullptr);
        clients_list_lock_.lock();
        int new_client = net::acceptClient(server_fd_);
        clients_fds_.push_back(new_client);
        clients_list_lock_.unlock();
        sendCurrentTime(new_client);
    }
}
