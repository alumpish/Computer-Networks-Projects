#include "timer.hpp"

Timer::Timer(const std::string& date) {
    parse(date, m_currentDate);
}

void Timer::addDays(int days) {
    m_currentDate += date::days(days);
}

// void Timer::printCurrentDate() const {
//     std::cout << "Current date: " << date::format("%F", m_currentDate) << "\n";
// }

date::sys_days Timer::getCurrentDate() const {
    return m_currentDate;
}