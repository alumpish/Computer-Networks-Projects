#ifndef TIMER_HPP
#define TIMER_HPP

#include <string>

#include "date.h"
#include "utils.hpp"

class Timer {
public:
    Timer();
    Timer(const std::string& date);
    void addDays(int days);
    date::sys_days getCurrentDate() const;
    void setTime(const std::string& date);

private:
    date::sys_days m_currentDate;
};

#endif