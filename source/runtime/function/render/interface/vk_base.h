#pragma once
#include "vk_start.h"

namespace nova {
class GraphicsBase {
private:
    GraphicsBase() = default;
    ~GraphicsBase() = default;

public:
    GraphicsBase(GraphicsBase&&) = delete;

    static GraphicsBase& instance() {
        static GraphicsBase singleton;
        return singleton;
    }

    VkInstance vk_instance;

    //设计一个同时具有vector和hash的容器
    std::vector<const char*> vk_instance_layers;
    std::vector<const char*> vk_instance_extensions;

    static void add_name_to_vector(const char* name, std::vector<const char*>& container) {
        for(auto& i:container) {
            if(!strcmp(name,i)) {
                return;
            }
        }
        container.push_back(name);
    }

    
    
    VkInstance get_vk_instance() const {
        return vk_instance;
    }

    
};
}