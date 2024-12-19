#include "LogSystem.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Nova
{
LogSystem::LogSystem()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    m_logger = spdlog::stdout_color_mt("Nova Runtime");
    m_logger->set_level(spdlog::level::trace);
}

LogSystem::~LogSystem()
{
    m_logger->flush();
    spdlog::drop_all();
}
}
