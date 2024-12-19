#include "RuntimeContext.h"

#include "Core/Log/LogSystem.h"

namespace Nova {
void RuntimeContext::Startup() {
    m_logSystem = std::make_shared<LogSystem>();
}

void RuntimeContext::Shutdown() {
    m_logSystem.reset();
}

}