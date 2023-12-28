// test/test_gtest_stress.cpp

#include <gtest/gtest.h>
#include "../src/Logger.hpp"
#include <thread>
#include <random>
#include <iostream>
#include <fstream>
#include <cstdio>  // 使用 std::remove 删除文件
// 获取文件大小的函数
std::streamsize getFileSize(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return -1;
    }

    std::streamsize size = file.tellg();
    file.close();

    return size;
}

// 删除文件的函数
bool deleteFile(const std::string& filename) {
    return std::remove(filename.c_str()) == 0;
}
// Test fixture class
class LoggerStressTest : public ::testing::Test {
protected:
    // Per-test-suite set-up
    static void SetUpTestSuite() {
        Logger::initializeLogger(true);
        Logger::setLogLevel(Logger::LogLevel::DEBUG);
        Logger::ConsoleOutputCtrl(true);
    }

    // Per-test-suite tear-down
    static void TearDownTestSuite() {
        Logger::shutdownLogger();
            const std::string filename = "Logfile.txt";

            // 获取文件大小
            std::streamsize size = getFileSize(filename);

            // 输出文件大小
            std::cout << "File size: " << size / 1024 /1024<< " MB" << std::endl;

            // 删除文件
            if (deleteFile(filename)) {
                std::cout << "File deleted: " << filename << std::endl;
            } else {
                std::cerr << "Error deleting file: " << filename << std::endl;
            }
    }
};

// Define the stress test function
void StressTestFunction() {
    const int iterations = 10000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> levelDistribution(static_cast<int>(Logger::LogLevel::ERROR),
                                                     static_cast<int>(Logger::LogLevel::DEBUG));


    for (int i = 0; i < iterations; ++i) {
        Logger::LogLevel level = static_cast<Logger::LogLevel>(levelDistribution(gen));
        switch (level) {
            case Logger::LogLevel::ERROR:
                LOG_ERROR("Thread ID: %d - Iteration: %d",std::this_thread::get_id(), i);
                break;
            case Logger::LogLevel::WARNING:
                LOG_WARNING("Thread ID: %d - Iteration: %d",std::this_thread::get_id(), i);
                break;
            case Logger::LogLevel::INFO:
                LOG_INFO("Thread ID: %d - Iteration: %d",std::this_thread::get_id(), i);
                break;
            case Logger::LogLevel::DEBUG:
                LOG_DEBUG("Thread ID: %d - Iteration: %d",std::this_thread::get_id(), i);
                break;
            default:
                break;
        }

        // Add more stress test assertions as needed
        ASSERT_TRUE(true);
    }
}




TEST_F(LoggerStressTest, StressTestAllLevels) {
    const int numThreads = 64;
    std::vector<std::thread> threads;

    // Launch multiple threads to simulate stress for each log level
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(StressTestFunction);
        threads.emplace_back(StressTestFunction);
        threads.emplace_back(StressTestFunction);
        threads.emplace_back(StressTestFunction);
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }
}

// Main function for gtest
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
