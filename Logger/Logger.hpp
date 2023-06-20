#pragma once
#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>

enum LogType
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

struct LogEntry
{
    LogType type;
    std::string message;
};

class Logger
{
    public:
        static void Log(const std::string &message);
        static void Err(const std::string &message);
        
        static std::vector<LogEntry> messages;
};
