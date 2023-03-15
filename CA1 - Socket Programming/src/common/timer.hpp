#ifndef TIMER_HPP
#define TIMER_HPP

#include "date/date.h"


class Timer {
public:
    Timer() : m_currentDate(date::floor<date::days>(date::current_time())) {}

    void addDays(int days);
    void printCurrentDate() const;

private:
    date::sys_days m_currentDate;
};

#endif