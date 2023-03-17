#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>

#include "timer.hpp"

class Logger {
public:
    Logger(Timer& timer);
    Logger(std::ostream* output_stream, Timer& timer);

    void setStream(std::ostream* output_stream);

    void info(const std::string& info, bool include_time = true);
    void warning(const std::string& warning, bool include_time = true);
    void error(const std::string& error, bool include_time = true);
    void footer(const std::string& footer);

private:
    std::ostream* output_stream_;
    Timer& timer_;

    void printMsg(const std::string& msg);
};

#endif