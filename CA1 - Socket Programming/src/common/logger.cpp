#include "logger.hpp"

#include <sstream>
#include <string>

Logger::Logger(std::ostream* output_stream)
    : output_stream_(output_stream) {}

void Logger::info(const std::string& info) {
    (*output_stream_) << "[INFO] ";
    printMsg(info);
}

void Logger::warning(const std::string& warning) {
    (*output_stream_) << "[WARN] ";
    printMsg(warning);
}

void Logger::error(const std::string& error) {
    (*output_stream_) << "[ERR] ";
    printMsg(error);
}

void Logger::printMsg(const std::string& msg) {
    (*output_stream_) << msg << std::endl;
}
