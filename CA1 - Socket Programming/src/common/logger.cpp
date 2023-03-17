#include "logger.hpp"

#include <sstream>
#include <string>

#include "timer.hpp"
#include "utils.hpp"

Logger::Logger(Timer& timer)
    : output_stream_(nullptr),
      timer_(timer) {}

Logger::Logger(std::ostream* output_stream, Timer& timer)
    : output_stream_(output_stream),
      timer_(timer) {}

void Logger::setStream(std::ostream* output_stream) {
    output_stream_ = output_stream;
}

void Logger::info(const std::string& info, bool include_time) {
    if (include_time)
        (*output_stream_) << to_string(timer_.getCurrentDate()) << std::endl;
    (*output_stream_) << "[INFO] ";
    printMsg(info);
}

void Logger::warning(const std::string& warning, bool include_time) {
    if (include_time)
        (*output_stream_) << to_string(timer_.getCurrentDate()) << std::endl;
    (*output_stream_) << "[WARN] ";
    printMsg(warning);
}

void Logger::error(const std::string& error, bool include_time) {
    if (include_time)
        (*output_stream_) << to_string(timer_.getCurrentDate()) << std::endl;
    (*output_stream_) << "[ERR] ";
    printMsg(error);
}

void Logger::footer(const std::string& footer) {
    printMsg(footer);
}

void Logger::printMsg(const std::string& msg) {
    (*output_stream_) << msg << std::endl;
}
