#include "api.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            void API::initialize() {
                ANDROMEDA_CORE_INFO("Initializing Vulkan API.");
                generate_instance();
            }
            void API::shutdown() {
                vkDestroyInstance(m_Instance, nullptr);
            }

            void API::generate_instance() {
                m_ApplicationInfo = {};
                m_ApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                m_ApplicationInfo.pApplicationName = "Waker [~] Online";
                m_ApplicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
                m_ApplicationInfo.pEngineName = "Andromeda";
                m_ApplicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
                m_ApplicationInfo.apiVersion = VK_API_VERSION_1_2;

                m_InstanceCreateInfo = {};
                m_InstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                m_InstanceCreateInfo.pApplicationInfo = & m_ApplicationInfo;

                m_GlfwExtensionCount = 0;
                m_GlfwExtensions = glfwGetRequiredInstanceExtensions(& m_GlfwExtensionCount);
                m_InstanceCreateInfo.enabledExtensionCount  = m_GlfwExtensionCount;
                m_InstanceCreateInfo.ppEnabledExtensionNames = m_GlfwExtensions;

#ifdef DEBUG
                const std::vector<const char *> m_ValidationLayers = {"VK_LAYER_KHRONOS_validation"};
                auto m_EnabledValidationLayers = check_validation_layer_support(m_ValidationLayers);
                m_InstanceCreateInfo.enabledLayerCount = m_EnabledValidationLayers.size();
                m_InstanceCreateInfo.ppEnabledLayerNames = m_EnabledValidationLayers.data();
                ANDROMEDA_CORE_INFO("Enabled Vulkan Validation Layers: ");
                for (auto & enabled_layer : m_EnabledValidationLayers) ANDROMEDA_CORE_TRACE("{0}", enabled_layer);
                ANDROMEDA_CORE_ASSERT(vkCreateInstance(& m_InstanceCreateInfo, nullptr, & m_Instance) == VK_SUCCESS, "Failed to create Vulkan API instance.");
                ANDROMEDA_CORE_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, & m_ExtensionCount, nullptr) == VK_SUCCESS, "Failed to count Vulkan extensions properties.");
                std::vector<VkExtensionProperties> m_Extensions(m_ExtensionCount);
                ANDROMEDA_CORE_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, & m_ExtensionCount, m_Extensions.data()) == VK_SUCCESS, "Failed to enumerate Vulkan extension properties.");
                ANDROMEDA_CORE_INFO("Vulkan Extensions: ");
                for (auto & extension : m_Extensions) ANDROMEDA_CORE_TRACE("{0}", extension.extensionName);
#elif
                m_InstanceCreateInfo.enabledLayerCount = 0;
                vkCreateInstance(& m_InstanceCreateInfo, nullptr, & m_Instance);
#endif
            }

            std::vector<const char *> API::check_validation_layer_support(std::vector<const char *> validation_layers) {
                unsigned int layer_count;
                vkEnumerateInstanceLayerProperties(& layer_count, nullptr);
                std::vector<VkLayerProperties> m_AvailableLayers(layer_count);
                std::vector<const char *> enabled_validation_layers;
                vkEnumerateInstanceLayerProperties(& layer_count, m_AvailableLayers.data());
                ANDROMEDA_CORE_INFO("Available Vulkan Validation Layers: ");
                for (auto & layer : m_AvailableLayers) ANDROMEDA_CORE_TRACE("{0}", layer.layerName);

                for (auto & validation_layer : validation_layers) {
                    auto result = std::find_if(m_AvailableLayers.begin(), m_AvailableLayers.end(),
                    [validation_layer](const VkLayerProperties & vkLayer) {
                        return strcmp(validation_layer, vkLayer.layerName) == 0;
                    });
                    if(result == m_AvailableLayers.end()) {
                        ANDROMEDA_CORE_ERROR("The {0} validation layer is not available; removing from chosen layers.", validation_layer);
                    } else {
                        enabled_validation_layers.push_back(validation_layer);
                    }
                }
                return enabled_validation_layers;
            }
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
