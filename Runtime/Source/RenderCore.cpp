#include "RenderCore.h"

namespace Nova {
void RenderCore::Init(RenderCoreInitInfo init_info) {
    m_window   = init_info.window_system->GetWindow();
    int width  = init_info.window_system->GetWidth();
    int height = init_info.window_system->GetHeight();

#ifndef NDEBUG
    m_enable_validation_layers = true;
#else
    m_enable_validation_layers = false;
#endif

    CreateInstance();
    InitDebugMessenger();
    CreateWindowSurface();
    InitPhysicalDevice();
    CreateLogicalDevice();
}
} // namespace Nova