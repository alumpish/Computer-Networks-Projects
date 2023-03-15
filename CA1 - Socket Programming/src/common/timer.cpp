#include "timer.hpp"


void Timer::addDays(int days) {
    m_currentDate += date::days(days);
}

void Timer::printCurrentDate() const {
    std::cout << "Current date: " << date::format("%F", m_currentDate) << "\n";
}