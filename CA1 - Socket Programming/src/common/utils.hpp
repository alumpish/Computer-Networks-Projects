#ifndef UTILS_HPP
#define UTILS_HPP

#include "date.h"


bool isNumber(const std::string& str);
bool parse(const std::string& date, date::sys_days& res);
std::string to_string(date::sys_days d);
bool isPasswordValid(const std::string& password);

#endif