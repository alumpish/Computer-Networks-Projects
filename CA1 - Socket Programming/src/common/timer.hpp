#ifndef TIMER_HPP
#define TIMER_HPP

#include <string>

#include "date.h"
#include "utils.hpp"

class Timer {
public:
    Timer(const std::string& date) {
        parse(date, m_currentDate);
    }

    void addDays(int days);
    // void printCurrentDate() const;
    date::sys_days getCurrentDate() const;

private:
    date::sys_days m_currentDate;
};

#endif