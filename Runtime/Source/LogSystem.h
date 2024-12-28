#pragma once

#include <spdlog/spdlog.h>
#include <memory>
#include <string>

namespace Nova {
class LogSystem {
private:
    std::shared_ptr<spdlog::logger> m_logger;

public:
    enum class LogLevel : uint8_t { Debug, Info, Warn, Error, Fatal };

public:
    LogSystem() = default;

    void Init();
    ~LogSystem();

    template<typename... TARGS>
    void Log(LogLevel level, fmt::format_string<TARGS...> fmt, TARGS&&... args) {
        switch (level) {
            case LogLevel::Debug:
                m_logger->debug(fmt, std::forward<TARGS>(args)...);
                break;
            case LogLevel::Info:
                m_logger->info(fmt, std::forward<TARGS>(args)...);
                break;
            case LogLevel::Warn:
                m_logger->warn(fmt, std::forward<TARGS>(args)...);
                break;
            case LogLevel::Error:
                m_logger->error(fmt, std::forward<TARGS>(args)...);
                break;
            case LogLevel::Fatal:
                m_logger->critical(fmt, std::forward<TARGS>(args)...);
                FatalCallback(fmt, std::forward<TARGS>(args)...);
                break;
            default:
                break;
        }
    }

    template<typename... TARGS>
    void FatalCallback(TARGS&&... args) {
        const std::string format_str = fmt::format(std::forward<TARGS>(args)...);
        throw std::runtime_error(format_str);
    }
};

} // namespace Nova