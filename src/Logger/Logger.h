#pragma once

#include <string>
#include <vector>

enum LogType {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

enum LogLevel {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_TEST
};

struct LogEntry {
    LogType type;
    LogLevel level;
    std::string message;
};

class Logger {
public:
    static LogLevel level;
    static std::vector<LogEntry> messages;
    static void Log(const std::string &message);
    static void LogD(const std::string &message);
    static void LogT(const std::string &message);
    static void Err(const std::string &message);
};