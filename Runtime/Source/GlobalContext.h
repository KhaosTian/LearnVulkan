#pragma once

#include <memory>
#include <string>

namespace Nova {

class LogSystem;
class WindowSystem;
class RenderSystem;

class GlobalContext {
private:
    std::shared_ptr<LogSystem> m_log_system;
    std::shared_ptr<WindowSystem> m_window_system;
    std::shared_ptr<RenderSystem> m_render_system;

public:
    void Startup();
    void Shutdown();

    std::shared_ptr<LogSystem> GetLogSystem() {
        return m_log_system;
    }

    std::shared_ptr<WindowSystem> GetWindowSystem() {
        return m_window_system;
    }

    std::shared_ptr<RenderSystem> GetRenderSystem() {
        return m_render_system;
    }
};

    extern GlobalContext kGlobalContext;
}
