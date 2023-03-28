#include "Logger.h"

#include <iostream>
#include <chrono>
#include <ctime>

std::vector<LogEntry> Logger::messages;
LogLevel Logger::level = LOG_LEVEL_TEST;


std::string CurrentDateTimeToString() {
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string output(30, '\0');
    std::strftime(&output[0], output.size(), "%d-%b-%y %H:%M:%S", std::localtime(&now));
    return output;
}

void Logger::Log(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "LOG: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}

void Logger::LogD(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.level = LOG_LEVEL_DEBUG;
    logEntry.message = "LOG_D: [" + CurrentDateTimeToString() + "]: " + message;
    if (level == LOG_LEVEL_DEBUG) {
        std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;
    }
}

void Logger::LogT(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.level = LOG_LEVEL_TEST;
    logEntry.message = "LOG_T: [" + CurrentDateTimeToString() + "]: " + message;
    if (level == LOG_LEVEL_TEST) {
        std::cout << "\x1B[36m" << logEntry.message << "\033[0m" << std::endl;
    }
    messages.push_back(logEntry);
}

void Logger::Err(const std::string& message) {
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERR: [" + CurrentDateTimeToString() + "]: " + message;
    std::cout << "\x1B[91m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}