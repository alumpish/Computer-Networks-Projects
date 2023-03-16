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

    void info(const std::string& info);
    void warning(const std::string& warning);
    void error(const std::string& error);
    void footer(const std::string& footer);

private:
    std::ostream* output_stream_;
    Timer& timer_;

    void printMsg(const std::string& msg);
};

#endif