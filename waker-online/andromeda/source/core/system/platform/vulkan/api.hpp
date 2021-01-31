#pragma once

#include "core/graphics/api.hpp"

#include <vulkan/vulkan.h>

#include <vector>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            struct API_Instance {
                VkApplicationInfo application_info;
                VkInstanceCreateInfo instance_create_info;
                VkInstance instance;
                VkResult status;
            };
            class API : public Andromeda::Graphics::API {
              public:
                virtual void initialize() override;
                virtual void shutdown() override;

              private:
                void generate_vulkan_instance();

              private:
                void create_application_info();
                void create_instance_create_info();
                VkResult create_instance();

                VkResult enumerate_instance_layer_properties(std::vector<VkLayerProperties> & instance_layer_properties);
                VkResult enumerate_instance_extension_properties(std::vector<VkExtensionProperties> & instance_extension_properties);
              private:
                bool check_desired_validation_layer_support(const std::vector<const char *> & desired_validation_layers);

              private:
                API_Instance m_API_Instance;
              private:
                const std::vector<const char *> desired_validation_layers = { "VK_LAYER_KHRONOS_validation" };
            };
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
