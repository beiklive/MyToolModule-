#include "../inc/log.hh"
#include "../inc/Common.h"



EXAMPLE_MAIN_START(log_main)

    beiklive::LOG::info("hello world");
    LOGGER_INFO("hello world");
    LOGGER_ERROR("this is error");
    LOGGER_DEBUG("this is debug 1");
    beiklive::LOG::LoggerLevelSet(beiklive::LOG::LOGLEVEL::DEBUG);
    LOGGER_DEBUG("this is debug 2");
    beiklive::LOG::LoggerStop();
    LOGGER_INFO("this log will not output");

EXAMPLE_MAIN_FINISH(log_main)