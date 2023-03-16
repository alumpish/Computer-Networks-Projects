#include "timer.hpp"

#include <string>

#include "date.h"
#include "exceptions.hpp"
#include "utils.hpp"

Timer::Timer() {}

Timer::Timer(const std::string& date) {
    parse(date, m_currentDate);
}

void Timer::addDays(int days) {
    m_currentDate += date::days(days);
}

date::sys_days Timer::getCurrentDate() const {
    return m_currentDate;
}

void Timer::setTime(const std::string& date) {
    if (!parse(date, m_currentDate))
        throw Err401();
}
