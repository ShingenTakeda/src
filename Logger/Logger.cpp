#include <Logger/Logger.hpp>

//TODO:Write Warning log function(make text yellow!).
//TODO:Use fmt for the output instead of std::cout.

std::vector<LogEntry> Logger::messages;

std::string CurrentTimeDateToString()
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string output(30, '\0');
    std::strftime(&output[0], output.size(), "%d-%b-%Y %H:%M:%S", std::localtime(&now));
    
    return output;
}

void Logger::Log (const std::string& message)
{
    LogEntry logEntry;
    logEntry.type = LOG_INFO;
    logEntry.message = "LOG: [" + CurrentTimeDateToString() + "]: " + message;
    std::cout << "\x1B[32m" << logEntry.message << "\033[0m" << std::endl;
    messages.push_back(logEntry);
}

void Logger::Err (const std::string& message)
{
    LogEntry logEntry;
    logEntry.type = LOG_ERROR;
    logEntry.message = "ERR: [" + CurrentTimeDateToString() + "]: " + message;
    messages.push_back(logEntry);
    std::cerr << "\x1B[91m"<< logEntry.message << "\033[0m" << std::endl;
}


