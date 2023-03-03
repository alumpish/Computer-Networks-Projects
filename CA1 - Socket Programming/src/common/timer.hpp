#ifndef TIMER_HPP
#define TIMER_HPP

#include <ctime>
#include <string>

class Timer {
public:
    // time format : dd-mm-yyyy
    void setTime(const std::string current_time);
    std::string getTime();

private:
    std::tm* system_time_;
    struct {
        int day;
        int month;
        int year;
    } given_date_;

    void updateDate();
    bool givenDateIsValid(int day, int month, int year) const;
};

#endif