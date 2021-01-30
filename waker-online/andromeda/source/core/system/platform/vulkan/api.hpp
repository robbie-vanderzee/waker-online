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
                virtual void shutdown() override;

              private:
                void generate_vulkan_instance();

              private:
                VkResult enumerate_instance_layer_properties(std::vector<VkLayerProperties> & instance_layer_properties);
                VkResult enumerate_instance_extension_properties(std::vector<VkExtensionProperties> & instance_extension_properties);
              private:
                bool check_desired_validation_layer_support(const std::vector<const char *> & desired_validation_layers);

              private:
                VkInstance m_Instance;
                VkApplicationInfo m_ApplicationInfo;
                VkInstanceCreateInfo m_InstanceCreateInfo;
                unsigned int m_ExtensionCount;

                unsigned int m_GlfwExtensionCount;
                const char ** m_GlfwExtensions;
            };
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
