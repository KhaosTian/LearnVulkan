#include "GlobalContext.h"

#include "LogSystem.h"
#include "WindowSystem.h"

namespace Nova {
GlobalContext kGlobalContext;

void GlobalContext::Startup() {
    m_log_system = std::make_shared<LogSystem>();
    m_log_system->Init();

    m_window_system = std::make_shared<WindowSystem>();
    WindowCreateInfo create_info;
    m_window_system-> Init(create_info);
}

void GlobalContext::Shutdown() {
    m_window_system.reset();

    m_log_system.reset();
}

} // namespace Nova