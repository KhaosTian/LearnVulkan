#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#include "memory"

namespace Vulkan {

class Log {
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetLogger() { return s_Logger; }

private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

} // namespace Vulkan

#define LOG_TRACE(...) ::Vulkan::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::Vulkan::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Vulkan::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Vulkan::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Vulkan::Log::GetLogger()->critical(__VA_ARGS__)

