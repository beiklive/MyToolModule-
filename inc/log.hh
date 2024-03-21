// Copyright (c) RealCoolEngineer. 2024. All rights reserved.
// Author: beiklive
// Date: 2024-01-31
#ifndef INC_LOG_HH_
#define INC_LOG_HH_

#include <iostream>
#include <sstream>
#include <iomanip>
#include <mutex>
#include <fstream>
#include <sys/stat.h>
#include <cstdlib>
#include <memory>
#ifdef _WIN32
#include <direct.h>
#else
#include <unistd.h>  // For Unix/Linux
#endif



namespace beiklive
{
    namespace LOG
    {
        class FileLogger;

        enum class LOGLEVEL
        {
            ERROR,
            WARNING,
            INFO,
            DEBUG
        };

        enum class OUTPUT
        {
            NONE,
            CONSOLE,
            FILE,
            ALL
        };

        enum class ColorCode
        {
            RESET = 0,
            DEBUG = 34,   // Blue
            INFO = 32,    // Green
            WARNING = 33, // Yellow
            ERROR = 31    // Red
        };


        class FileLogger {
        public:
            FileLogger() : logFile(nullptr) {}

            ~FileLogger() {
                if (logFile && logFile->is_open()) {

                    // 刷新并关闭文件
                    flush();
                    logFile->close();
                }
            }

            void initializeLogFile(const std::string& filePath) {
                logFile.reset(new std::ofstream(filePath, std::ios::app));

                if (!logFile->is_open()) {
                    std::cerr << "Error opening log file: " << filePath << std::endl;
                }
                else {
                    currentFilePath = filePath;
                    // 设置缓冲区大小
                    logFile->rdbuf()->pubsetbuf(0, 1024);

                }
            }

            void logMessage(const std::string& message) {
                if (logFile && logFile->is_open()) {
                    (*logFile) << message  << std::endl;
                }
            }

            void switchLogFile(const std::string& newFilePath) {
                if (logFile && logFile->is_open()) {
                    // 刷新并关闭当前文件
                    flush();
                    logFile->close();

                    // 初始化新文件
                    initializeLogFile(newFilePath);
                }
            }

        private:
            std::unique_ptr<std::ofstream> logFile;
            std::string currentFilePath;

            void flush() {
                if (logFile && logFile->is_open()) {
                    logFile->flush();
                }
            }
        };


        namespace
        {
            LOGLEVEL        loglevel_ = LOGLEVEL::INFO;
            OUTPUT          output_ = OUTPUT::CONSOLE;

            std::string     logFilePath_ = "./log";
            std::string     CurLogFile_;
            std::string     CurCycleLogDirName_;
            long long       MaxSingleLogFileSize_ = 1024 * 1024 * 10; // 10MB
            FileLogger      filelogger;

            std::mutex      logMutex;
        }

        //*FILE ***************************************************************
        std::string generateLogFileName() {
            auto now = std::chrono::system_clock::now();
            auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()
            ).count() % 1000;

            // 获取当前日期和时间
            auto timeNow = std::chrono::system_clock::to_time_t(now);
            std::tm tmNow = *std::localtime(&timeNow);

            // 格式化日期和时间
            std::stringstream ss;
            ss << std::put_time(&tmNow, "%Y%m%d_%H_%M_%S_") << std::setw(3) << std::setfill('0') << millisec;

            return ss.str();
        }


        bool createDirectory(const std::string& directoryPath) {
            // 检查目录是否已存在
            if (access(directoryPath.c_str(), 0) == 0) {
                std::cout << "Directory already exists: " << directoryPath << std::endl;
                return true;
            }

        #ifdef _WIN32
            int result = _mkdir(directoryPath.c_str());
        #else
            mode_t mode = 0755;  // 权限设置为rwxr-xr-x
            int result = mkdir(directoryPath.c_str(), mode);
        #endif

            if (result == 0) {
                std::cout << "Directory created successfully: " << directoryPath << std::endl;
                return true;
            } else {
                std::perror(std::string("Error creating directory: " + directoryPath).c_str());
                return false;
            }
        }


        bool isFileSizeOverSize(const std::string& filename) {

            std::lock_guard<std::mutex> lock(logMutex);
            std::ifstream file(filename, std::ios::binary | std::ios::ate);
            if (!file.is_open()) {
                std::cerr << "Error opening file: " << filename << std::endl;
                return false;  // Unable to open the file
            }
            std::streamsize fileSize = file.tellg();  // Get the file size
            file.close();  // Close the file
            return fileSize > MaxSingleLogFileSize_;
        }

        void endsWithSlash(std::string& str) {
            if (!str.empty()) {
                char lastChar = str.back();
                if (lastChar == '/') {
                    return;
                }
                else {
                    str += '/';
                }
            }
        }

        void LogFileRotation(const std::string& msg)
        {
            // 目录初始化
            if (CurCycleLogDirName_.empty())
            {
                createDirectory(logFilePath_);
                CurCycleLogDirName_ = generateLogFileName();
                endsWithSlash(logFilePath_);
                createDirectory(logFilePath_ + CurCycleLogDirName_);
            }

            if (CurLogFile_.empty())
            {
                CurLogFile_ = generateLogFileName() + ".log";
                std::cout << "New logfile : " << CurLogFile_ << std::endl;
                filelogger.initializeLogFile(logFilePath_ + CurCycleLogDirName_ + "/" + CurLogFile_);
            }

            if (isFileSizeOverSize(logFilePath_ + CurCycleLogDirName_ + "/" + CurLogFile_))
            {
                CurLogFile_ = generateLogFileName() + ".log";
                std::cout << "Switch to new logfile : " << CurLogFile_ << std::endl;
                filelogger.switchLogFile(logFilePath_ + CurCycleLogDirName_ + "/" + CurLogFile_);
            }

            filelogger.logMessage(msg);
        }

        void LogFileSizeSet(const long& maxSize = 1024 * 1024 * 10)
        {
            MaxSingleLogFileSize_ = maxSize;
        }

        void LogFilePathSet(const std::string& dirPath)
        {
            if(createDirectory(dirPath))
            {
                logFilePath_ = dirPath;
            }
            std::cout << "Use log path : " << logFilePath_ << std::endl;
        }
        //***************************************************************


        void LoggerOutputSet(const OUTPUT set)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            output_ = set;
        }

        void LoggerLevelSet(const LOGLEVEL set)
        {
            std::lock_guard<std::mutex> lock(logMutex);
            loglevel_ = set;
        }

        void LoggerStop()
        {
            std::lock_guard<std::mutex> lock(logMutex);
            output_ = OUTPUT::NONE;
        }

        bool isEnableOutput()
        {
            return !(output_ == OUTPUT::NONE);
        }

        bool isConsoleOutput() {
            return (output_ == OUTPUT::CONSOLE || output_ == OUTPUT::ALL);
        }

        bool isFileOutput()
        {
            return (output_ == OUTPUT::FILE || output_ == OUTPUT::ALL);
        }

#define COLOR(level, message) \
        (std::string("\033[") + std::to_string(static_cast<int>(level)) + "m" + message + "\033[0m")

#define GET_FUNCTION_NAME() (std::string(__PRETTY_FUNCTION__) + ":" + std::to_string(__LINE__))

        std::string getCurrentTimestamp()
        {
            auto now = std::chrono::system_clock::now();
            auto currentTime = std::chrono::system_clock::to_time_t(now);
            auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()
            ).count() % 1000;

            std::tm tmNow = *std::localtime(&currentTime);

            std::stringstream timestamp;
            timestamp << std::put_time(&tmNow, "%Y-%m-%d %H:%M:%S.") << std::setw(3) << std::setfill('0') << millisec;;

            return timestamp.str();
        }


        std::string format(const std::string& pattern)
        {
            return pattern;
        }

        template <typename T, typename... Args>
        std::string format(const std::string& pattern, T first, Args &&...args)
        {
            std::stringstream ss;
            size_t pos = pattern.find("{");
            size_t pos2 = pattern.find("}", pos);

            if (pos != std::string::npos && pos2 != std::string::npos)
            {
                ss << pattern.substr(0, pos) << first << pattern.substr(pos2 + 1);
            }
            else
            {
                ss << pattern;
            }

            return format(ss.str(), args...);
        }

        template <typename T, typename K, typename... Args>
        void MACRO_LOG_OUTPUT(const LOGLEVEL level, T first, K pattern, Args &&...args)
        {
            if (isEnableOutput())
            {
                if (loglevel_ >= level)
                {
                    std::stringstream ss;
                    ss << "[";
                    ss << first;
                    ss << "] ";
                    ss << pattern;
                    LOG_OUTPUT(level, ss.str(), args...);
                }
            }
        }

        template <typename... Args>
        void LOG_OUTPUT(const LOGLEVEL level, const std::string& pattern, Args &&...args)
        {
            std::string&& s = format(pattern, args...);


            std::stringstream ss;
            ss << "[" << getCurrentTimestamp() << "]";
            ss << " ";
            if (isConsoleOutput()) {
                std::stringstream sss;
                sss << ss.str();
                switch (level)
                {
                case LOGLEVEL::INFO:
                    sss << "[" << COLOR(ColorCode::INFO, "INFO") << "]";
                    break;
                case LOGLEVEL::WARNING:
                    sss << "[" << COLOR(ColorCode::WARNING, "WARNING") << "]";
                    break;
                case LOGLEVEL::ERROR:
                    sss << "[" << COLOR(ColorCode::ERROR, "ERROR") << "]";
                    break;
                case LOGLEVEL::DEBUG:
                    sss << "[" << COLOR(ColorCode::DEBUG, "DEBUG") << "]";
                    break;
                }
                sss << " ";
                sss << s;
                std::cout << sss.str() << std::endl;
            }
            if (isFileOutput())
            {
                std::stringstream sss;
                sss << ss.str();
                switch (level)
                {
                case LOGLEVEL::INFO:
                    sss << "[I]";
                    break;
                case LOGLEVEL::WARNING:
                    sss << "[W]";
                    break;
                case LOGLEVEL::ERROR:
                    sss << "[E]";
                    break;
                case LOGLEVEL::DEBUG:
                    sss << "[D]";
                    break;
                }
                sss << " ";
                sss << s;
                LogFileRotation(sss.str());
            }
        }

        template <typename T, typename... Args>
        void info(T pattern, Args &&...args)
        {
            if (loglevel_ >= LOGLEVEL::INFO && isEnableOutput())
                LOG_OUTPUT(LOGLEVEL::INFO, pattern, args...);
        }
        template <typename T, typename... Args>
        void warning(T pattern, Args &&...args)
        {
            if (loglevel_ >= LOGLEVEL::WARNING && isEnableOutput())
                LOG_OUTPUT(LOGLEVEL::WARNING, pattern, args...);
        }
        template <typename T, typename... Args>
        void error(T pattern, Args &&...args)
        {
            if (loglevel_ >= LOGLEVEL::ERROR && isEnableOutput())
                LOG_OUTPUT(LOGLEVEL::ERROR, pattern, args...);
        }
        template <typename T, typename... Args>
        void debug(T pattern, Args &&...args)
        {
            if (loglevel_ >= LOGLEVEL::DEBUG && isEnableOutput())
                LOG_OUTPUT(LOGLEVEL::DEBUG, pattern, args...);
        }

    } // namespace log
#define LOG_INFO(...) LOGGER_INFO(__VA_ARGS__)
#define LOG_WARNING(...) LOGGER_WARNING(__VA_ARGS__)
#define LOG_ERROR(...) LOGGER_ERROR(__VA_ARGS__)
#define LOG_DEBUG(...) LOGGER_DEBUG(__VA_ARGS__)

#define LOGGER_INFO(...) MACRO_LOG_OUTPUT(beiklive::LOG::LOGLEVEL::INFO, GET_FUNCTION_NAME(), __VA_ARGS__)
#define LOGGER_WARNING(...) MACRO_LOG_OUTPUT(beiklive::LOG::LOGLEVEL::WARNING, GET_FUNCTION_NAME(), __VA_ARGS__)
#define LOGGER_ERROR(...) MACRO_LOG_OUTPUT(beiklive::LOG::LOGLEVEL::ERROR, GET_FUNCTION_NAME(), __VA_ARGS__)
#define LOGGER_DEBUG(...) MACRO_LOG_OUTPUT(beiklive::LOG::LOGLEVEL::DEBUG, GET_FUNCTION_NAME(), __VA_ARGS__)

} // namespace beiklive


#endif  // INC_LOG_HH_

