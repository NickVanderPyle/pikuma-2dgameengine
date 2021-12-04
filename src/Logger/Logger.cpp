#include "Logger.h"
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>

std::string CurrentDateTimeToString() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string output(30, '\0');
    std::strftime(&output[0], output.size(), "%Y-%b-%dT%H:%M:%S", gmtime(&now));
    return output;
}

void Logger::Log(const std::string &message) {
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "LOG [" + CurrentDateTimeToString() + "]: " + message;
    std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;
}

void Logger::Err(const std::string &message) {
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERR [" + CurrentDateTimeToString() + "]: " + message;
    std::cerr << "\x1B[91m" << logEntry.message << "\033[0m" << std::endl;
}
