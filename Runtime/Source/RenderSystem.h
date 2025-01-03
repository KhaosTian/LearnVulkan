#pragma once

#include <memory>
#include <optional>

namespace Nova {
class WindowSystem;
class VulkanCore;

struct RenderSystemInitInfo {
    std::shared_ptr<WindowSystem> window_system;
};

class RenderSystem {
private:
    std::shared_ptr<VulkanCore> m_render_core;

public:
    RenderSystem() = default;
    ~RenderSystem();

    void Init(RenderSystemInitInfo init_info);
    void Tick(float delta_time);
    void Clear();
};
}
