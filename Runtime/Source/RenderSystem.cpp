#include "RenderSystem.h"

#include "VulkanCore.h";

namespace Nova {
RenderSystem::~RenderSystem() {
    Clear();
}

void RenderSystem::Init(RenderSystemInitInfo init_info) {
    VulkanCoreInitInfo core_init_info;
    core_init_info.window_system = init_info.window_system;

    m_render_core = std::make_shared<VulkanCore>();
    m_render_core->Init(core_init_info);
}
void RenderSystem::Tick(float delta_time) {}
void RenderSystem::Clear() {
    m_render_core->Clear();
}
} // namespace Nova
