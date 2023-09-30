#include "timer.hpp"

#include <string>

#include "date.h"
#include "exceptions.hpp"
#include "utils.hpp"

Timer::Timer() {}

Timer::Timer(const std::string& date) {
    current_date_ = Timer::parseString(date);
}

void Timer::addDays(int days) {
    current_date_ += date::days(days);
}

date::sys_days Timer::getCurrentDate() const {
    return current_date_;
}

void Timer::setTime(const std::string& date) {
    current_date_ = Timer::parseString(date);
}

date::sys_days Timer::parseString(const std::string& date) {
    date::sys_days ymd;
    std::istringstream ss(date);
    ss >> date::parse("%d-%m-%Y", ymd);
    if (ss.fail()) {
        throw Err401();
    }
    return ymd;
}

std::string Timer::dateToString(const date::sys_days& date) {
    std::ostringstream ss;
    ss << date::format("%d-%m-%Y", date);
    return ss.str();
}
