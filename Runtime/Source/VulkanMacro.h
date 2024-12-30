#pragma once

#include "LogSystem.h"

#include "GlobalContext.h"

// 分离格式字符串和参数的版本
#define LOG_HELPER(LOG_LEVEL, FMT, ...)\
    Nova::kGlobalContext.GetLogSystem()->Log(LOG_LEVEL, "[{}] " FMT, __FUNCTION__, ##__VA_ARGS__)

#define LOG_DEBUG(FMT, ...)\
    LOG_HELPER(Nova::LogSystem::LogLevel::Debug, FMT, ##__VA_ARGS__)

#define LOG_INFO(FMT, ...)\
    LOG_HELPER(Nova::LogSystem::LogLevel::Info, FMT, ##__VA_ARGS__)

#define LOG_WARN(FMT, ...)\
    LOG_HELPER(Nova::LogSystem::LogLevel::Warn, FMT, ##__VA_ARGS__)

#define LOG_ERROR(FMT, ...)\
    LOG_HELPER(Nova::LogSystem::LogLevel::Error, FMT, ##__VA_ARGS__)

#define LOG_FATAL(FMT, ...)\
    LOG_HELPER(Nova::LogSystem::LogLevel::Fatal, FMT, ##__VA_ARGS__)


#ifdef NDEBUG
    #define ASSERT(statement)
#else
    #define ASSERT(statement) assert(statement)
#endif