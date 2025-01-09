#include "Vulkan.hpp"

namespace Vulkan {
void Check(const VkResult result, const char* const operation) {
    if (result != VK_SUCCESS) {
        LOG_ERROR(std::runtime_error(std::string("Failed to ") + operation));
    }
}

} // namespace Vulkan