#pragma once

#include "core/graphics/api.hpp"

#include <vulkan/vulkan.h>

#include <vector>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            class API : public Andromeda::Graphics::API {
              public:
                virtual void initialize() override;
                virtual void process() override;
                virtual void shutdown() override;

              public:
                void generate_instance();
              private:
                VkInstance m_Instance;
                VkApplicationInfo m_ApplicationInfo;
                VkInstanceCreateInfo m_InstanceCreateInfo;
                unsigned int m_ExtensionCount;

                std::vector<VkExtensionProperties> m_Extensions;


                unsigned int m_GlfwExtensionCount;
                const char ** m_GlfwExtensions;
            };
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
