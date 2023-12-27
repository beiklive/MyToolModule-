/*
   Logger.hpp - A simple C++ logging utility

   Description:
   This header file provides a logging utility that allows you to log messages
   with different log levels (ERROR, WARNING, INFO, DEBUG). The logs can be
   directed to both the console and a log file. Log messages are timestamped
   and can be colored for better visibility in the console.

   Features:
   - Log levels: ERROR, WARNING, INFO, DEBUG
   - Colored output for different log levels in the console
   - Timestamps for each log entry
   - Ability to log to a file with optional timestamp and log level

   Usage:
   - Include this header file in your C++ project.
   - Initialize the logger using Logger::initializeLogger(). Optionally, you can
     specify whether to enable logging to a file.
   - Log messages using macros like LOG_ERROR, LOG_WARNING, LOG_INFO, LOG_DEBUG.
   - Set the log level using Logger::setLogLevel() to control which messages
     are actually logged.
   - Shut down the logger with Logger::shutdownLogger() when your program ends.

   Example:
   
   #include "Logger.hpp"

   int main() {
       Logger::initializeLogger();
       Logger::setLogLevel(Logger::LogLevel::DEBUG);

       LOG_ERROR("This is an error message");
       LOG_WARNING("This is a warning message");
       LOG_INFO("This is an info message");
       LOG_DEBUG("This is a debug message");

       Logger::shutdownLogger();
       return 0;
   }
   

   Command Line Options:
   - You can specify the log level at runtime using the command line option
     `--log-level`. For example, `--log-level DEBUG` sets the log level to DEBUG.
    
    Example:
        Logger::setLogLevel(Logger::CommandLine::parseLogLevel(argc, argv));


   Platform-specific considerations:
   - The code includes platform-specific directives for handling timestamps
     based on whether it is compiled for Windows or other operating systems.

   Notes:
   - This logging utility uses C++11 features.
   - Make sure to flush the log file buffer after each log entry by calling
     `logFile.flush()` if you want to see the log entries immediately in the file.

   Author:
   beiklive
   1376896787@qq.com
   2023年12月27日
*/



#ifndef __LOGGER__
#define __LOGGER__
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <mutex>
#include <cstdarg>
#include <chrono>

namespace Logger {
    enum LogLevel {
        ERROR,
        WARNING,
        INFO,
        DEBUG
    };

    namespace {
        LogLevel logLevel = LogLevel::INFO;
        std::ofstream logFile;
        std::mutex logMutex;
        bool logFileEnabled = true;
    }

    void initializeLogger(bool enableLogFile = true) {
        logFileEnabled = enableLogFile;

        if (logFileEnabled) {
            std::lock_guard<std::mutex> lock(logMutex);
            logFile.open("logfile.txt", std::ios::app);
        }
    }

    void shutdownLogger() {
        std::lock_guard<std::mutex> lock(logMutex);
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void setLogLevel(LogLevel level) {
        logLevel = level;
    }

    std::string getLogLevelString(LogLevel level) {
        switch (level) {
            case LogLevel::ERROR: return   "\033[1;31mERROR\033[0m";  // 红色
            case LogLevel::WARNING: return "\033[1;33mWARNING\033[0m";  // 黄色
            case LogLevel::INFO: return    "\033[1;32mINFO\033[0m";  // 绿色
            case LogLevel::DEBUG: return   "\033[1;34mDEBUG\033[0m";  // 蓝色
            default: return "UNKNOWN";
        }
    }
    std::string getLogLevelStringNoColor(LogLevel level) {
        switch (level) {
            case LogLevel::ERROR: return "E";  // 红色
            case LogLevel::WARNING: return "W";  // 黄色
            case LogLevel::INFO: return "I";  // 绿色
            case LogLevel::DEBUG: return "D";  // 蓝色
            default: return "UNKNOWN";
        }
    }
    template <typename T>
    std::string formatLogMessage(const T& value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    template <typename T, typename... Args>
    std::string formatLogMessage(const T& value, const Args&... args) {
        return formatLogMessage(value) + formatLogMessage(args...);
    }

    std::string getTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

        std::ostringstream oss;
        
#ifdef _WIN32
        std::tm tm = {};
        localtime_s(&tm, &time_t_now);
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm);
        oss << timestamp << "." << ms.count();
#else
        std::tm tm;
        localtime_r(&time_t_now, &tm);
        char timestamp[32];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &tm);
        oss << timestamp << "." << ms.count();
#endif

        return oss.str();
    }

    void log(LogLevel level, const char* format, ...) {
        if (level <= logLevel) {
            std::lock_guard<std::mutex> lock(logMutex);

            va_list args;
            va_start(args, format);

            char buffer[256];
            vsnprintf(buffer, sizeof(buffer), format, args);

            va_end(args);

            // 输出到控制台
            std::cout << "[" << getTimestamp() << "] [" << getLogLevelString(level) << "] " << buffer << std::endl;

            // 输出到文件
            if (logFileEnabled && logFile.is_open()) {
                logFile << "[" << getTimestamp() << "] [" << getLogLevelStringNoColor(level) << "] " << buffer << std::endl;
                logFile.flush();  // 刷新文件缓冲区
            }
        }
    }

    namespace CommandLine {
        LogLevel parseLogLevel(int argc, char* argv[]) {
            LogLevel level = LogLevel::INFO;

            for (int i = 1; i < argc; ++i) {
                std::string arg = argv[i];

                if (arg == "--log-level" && i + 1 < argc) {
                    std::string levelStr = argv[i + 1];

                    if (levelStr == "ERROR") {
                        level = LogLevel::ERROR;
                    } else if (levelStr == "WARNING") {
                        level = LogLevel::WARNING;
                    } else if (levelStr == "INFO") {
                        level = LogLevel::INFO;
                    } else if (levelStr == "DEBUG") {
                        level = LogLevel::DEBUG;
                    } else {
                        std::cerr << "Invalid log level specified. Using default level (INFO)." << std::endl;
                    }
                }
            }

            return level;
        }
    }
}

#define LOG_ERROR(...) Logger::log(Logger::LogLevel::ERROR, __VA_ARGS__)
#define LOG_WARNING(...) Logger::log(Logger::LogLevel::WARNING, __VA_ARGS__)
#define LOG_INFO(...) Logger::log(Logger::LogLevel::INFO, __VA_ARGS__)
#define LOG_DEBUG(...) Logger::log(Logger::LogLevel::DEBUG, __VA_ARGS__)

#endif