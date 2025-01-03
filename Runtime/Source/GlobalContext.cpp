#include "GlobalContext.h"

#include "LogSystem.h"
#include "WindowSystem.h"
#include "RenderSystem.h"

namespace Nova {
GlobalContext kGlobalContext;

void GlobalContext::Startup() {
    m_log_system = std::make_shared<LogSystem>();
    m_log_system->Init();

    m_window_system = std::make_shared<WindowSystem>();
    WindowCreateInfo window_create_info;
    m_window_system->Init(window_create_info);

    m_render_system = std::make_shared<RenderSystem>();
    RenderSystemInitInfo render_init_info;
    render_init_info.window_system = m_window_system;
    m_render_system->Init(render_init_info);
}

void GlobalContext::Shutdown() {
    m_render_system.reset();

    m_window_system.reset();

    m_log_system.reset();
}

} // namespace Nova