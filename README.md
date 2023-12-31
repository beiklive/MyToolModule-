# 项目名称

这个项目提供了一个简单的多线程C++日志记录库，用于方便地在应用程序中添加日志记录功能。

## 目录

- [项目名称](#项目名称)
  - [目录](#目录)
  - [项目结构](#项目结构)
  - [说明](#说明)
  - [使用](#使用)
  - [构建和运行](#构建和运行)
  - [测试](#测试)

## 项目结构

```bash
├── run.sh
├── src
│   ├── Logger.hpp
│   └── main.cpp
├── test
│   └── test_gtest.cpp
└── xmake.lua
```

- `run.sh`: 运行项目的Shell脚本。
- `src`: 包含源代码的目录。
  - `Logger.hpp`: 日志记录器头文件。
  - `main.cpp`: 主要源文件。
- `test`: 包含测试文件的目录。
  - `test_gtest.cpp`: 用于测试的Google Test文件。
- `xmake.lua`: Xmake构建脚本。

## 说明

该日志记录库具有四个日志级别：ERROR、WARNING、INFO 和 DEBUG。它还支持将日志输出到控制台和文件中。

## 使用

要在项目中使用日志记录库，只需包含 `Logger.hpp` 头文件，并使用以下宏进行日志记录：

```cpp
LOG_ERROR("This is an error message");
LOG_WARNING("This is a warning message");
LOG_INFO("This is an info message");
LOG_DEBUG("This is a debug message");
```

## 构建和运行

```bash
# 示例构建和运行命令
./run.sh
```

## 测试

解释如何运行测试以及有关测试过程的任何其他信息。

```bash
# 示例测试命令
xmake && xmake run test_gtest
```

使用256线程，每线程 10000 条输出，压测结果如下


关闭控制台打印
```
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from LoggerStressTest
[ RUN      ] LoggerStressTest.StressTestAllLevels
[       OK ] LoggerStressTest.StressTestAllLevels (12377 ms)
File size: 169 MB
File deleted: Logfile.txt
[----------] 1 test from LoggerStressTest (12377 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (12401 ms total)
[  PASSED  ] 1 test.
```
开启控制台打印
```
[==========] Running 1 test from 1 test suite.
[----------] Global test environment set-up.
[----------] 1 test from LoggerStressTest
[ RUN      ] LoggerStressTest.StressTestAllLevels
[       OK ] LoggerStressTest.StressTestAllLevels (155878 ms)
File size: 169 MB
File deleted: Logfile.txt
[----------] 1 test from LoggerStressTest (155878 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test suite ran. (155897 ms total)
[  PASSED  ] 1 test.
```