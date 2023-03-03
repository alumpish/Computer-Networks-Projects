#include "timer.hpp"

#include <ctime>
#include <iomanip>
#include <regex>
#include <sstream>
#include <string>

#include "consts.hpp"
#include "exceptions.hpp"

void Timer::setTime(const std::string current_time) {
    std::regex rgx(Consts::Timer::DATE_FORMAT);
    std::smatch matches;

    if (!std::regex_search(current_time, matches, rgx))
        throw Err401();

    int current_day = std::stoi(
        matches[0].str()
    );

    int current_month = std::stoi(
        matches[1].str()
    );

    int current_year = std::stoi(
        matches[2].str()
    );

    if (!givenDateIsValid(current_day, current_month, current_year))
        throw Err401();

    std::time_t cur_sys_time = std::time(nullptr);
    system_time_ = std::localtime(&cur_sys_time);

    given_date_ = {current_day, current_month, current_year};
}

std::string Timer::getTime() {
    updateDate();
    std::ostringstream result;
    result << std::setw(2) << std::setfill('0') << given_date_.day << '-' << given_date_.month << '-' << std::setw(4) << given_date_.year;
    return result.str();
}

void Timer::updateDate() {
    std::time_t cur_sys_time = std::time(nullptr);
    std::tm* cur_sys_tm = std::localtime(&cur_sys_time);
    given_date_.year += cur_sys_tm->tm_year - system_time_->tm_year;
    given_date_.month += cur_sys_tm->tm_mon - system_time_->tm_mon;
    given_date_.day += cur_sys_tm->tm_mday - system_time_->tm_mday;
}

bool Timer::givenDateIsValid(int day, int month, int year) const {
    if (month <= 0 || day <= 0 || year <= 0)
        return false;
    if (month == 1)
        return day <= 31;
    if (month == 2)
        return day <= 28;
    if (month == 3)
        return day <= 31;
    if (month == 4)
        return day <= 30;
    if (month == 5)
        return day <= 31;
    if (month == 6)
        return day <= 30;
    if (month == 7) 
        return day <= 31;
    if (month == 8)
        return day <= 31;
    if (month == 9)
        return day <= 30;
    if (month == 10)
        return day <= 31;
    if (month == 11)
        return day <= 30;
    if (month == 12)
        return day <= 31;
}
