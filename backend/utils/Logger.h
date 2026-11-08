#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <sstream>

/**
 * ============================================================================
 * OOP CONCEPT: Static Members, Thread Safety & Encapsulation
 * Logger provides unified timestamped logging to console/audit trail.
 * ============================================================================
 */
class Logger {
public:
    enum class Level { INFO, WARNING, ERR, DEBUG_LVL };

    static void log(Level level, const std::string& message) {
        std::lock_guard<std::mutex> lock(s_mutex);
        std::string prefix;
        switch (level) {
            case Level::INFO: prefix = "[INFO]"; break;
            case Level::WARNING: prefix = "[WARN]"; break;
            case Level::ERR: prefix = "[ERROR]"; break;
            case Level::DEBUG_LVL: prefix = "[DEBUG]"; break;
        }

        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm time_info;
#if defined(_WIN32)
        localtime_s(&time_info, &in_time_t);
#else
        localtime_r(&in_time_t, &time_info);
#endif
        std::cout << std::put_time(&time_info, "%Y-%m-%d %H:%M:%S")
                  << " " << prefix << " " << message << std::endl;
    }

    static void info(const std::string& msg) { log(Level::INFO, msg); }
    static void warn(const std::string& msg) { log(Level::WARNING, msg); }
    static void error(const std::string& msg) { log(Level::ERR, msg); }
    static void debug(const std::string& msg) { log(Level::DEBUG_LVL, msg); }

private:
    static std::mutex s_mutex;
};

#endif // LOGGER_H
