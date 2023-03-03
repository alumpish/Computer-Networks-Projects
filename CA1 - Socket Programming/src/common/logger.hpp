#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>

class Logger {
public:
    Logger() = delete;
    Logger(std::ostream* output_stream);
    void info(const std::string& info);
    void warning(const std::string& warning);
    void error(const std::string& error);

private:
    std::ostream* output_stream_;

    void printMsg(const std::string& msg);
};

#endif