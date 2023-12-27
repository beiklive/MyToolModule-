#include <iostream>
#include "Logger.hpp"
using namespace std;



int main(int argc, char* argv[]) {
    // 设置全局log等级
    Logger::setLogLevel(Logger::CommandLine::parseLogLevel(argc, argv));
    Logger::initializeLogger(true);
    // 使用简化的log宏记录日志
    LOG_ERROR("This is an error message.");
    LOG_WARNING("This is a warning message.");
    LOG_INFO("This is an info message.");
    LOG_DEBUG("This is a debug message.");

    // 使用带有格式化参数的log宏记录日志
    LOG_ERROR("Error: %d - %0.2f", 42, 3.14);
    LOG_DEBUG("Debug: Value is %d", 42);


    return 0;
}