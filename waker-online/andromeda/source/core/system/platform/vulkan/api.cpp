#include "api.hpp"

#include <GLFW/glfw3.h>

#include <cstring>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            void API::initialize() {
                ANDROMEDA_CORE_INFO("Initializing Vulkan API.");
                generate_vulkan_instance();
            }
            void API::shutdown() {
                vkDestroyInstance(m_Instance, nullptr);
            }

            void API::generate_vulkan_instance() {
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
                const std::vector<const char *> desired_validation_layers = { "VK_LAYER_KHRONOS_validation" };
                bool check_desired_validation_layer_support_status = check_desired_validation_layer_support(desired_validation_layers);
                ANDROMEDA_CORE_ASSERT(check_desired_validation_layer_support_status, "Failed to apply all Vulkan validation layers.");
                m_InstanceCreateInfo.enabledLayerCount = desired_validation_layers.size();
                m_InstanceCreateInfo.ppEnabledLayerNames = desired_validation_layers.data();

                ANDROMEDA_CORE_INFO("Enabled Vulkan Validation Layers: ");
                for (auto & enabled_validation_layer : desired_validation_layers) ANDROMEDA_CORE_TRACE("{0}", enabled_validation_layer);

                std::vector<VkExtensionProperties> instance_extension_properties;
                VkResult enumerated = enumerate_instance_extension_properties(instance_extension_properties);
                ANDROMEDA_CORE_ASSERT(enumerated == VK_SUCCESS, "Failed to enumate Vulkan extension layer properties.");

                ANDROMEDA_CORE_INFO("Vulkan Extensions: ");
                for (auto & extension : instance_extension_properties) ANDROMEDA_CORE_TRACE("{0}", extension.extensionName);

                VkResult create_instance_status = vkCreateInstance(& m_InstanceCreateInfo, nullptr, & m_Instance);
                ANDROMEDA_CORE_ASSERT(create_instance_status == VK_SUCCESS, "Failed to create Vulkan API instance.");
#else
                m_InstanceCreateInfo.enabledLayerCount = 0;
                VkResult create_instance_status = vkCreateInstance(& m_InstanceCreateInfo, nullptr, & m_Instance);
                ANDROMEDA_CORE_ASSERT(create_instance_status == VK_SUCCESS, "Failed to create Vulkan API instance.");
#endif
            }

            VkResult API::enumerate_instance_extension_properties(std::vector<VkExtensionProperties> & instance_extension_properties) {
                unsigned int extension_count;
                VkResult count_instance_extension_properties_status = vkEnumerateInstanceExtensionProperties(nullptr, & extension_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_instance_extension_properties_status == VK_SUCCESS, "Failed to count Vulkan extensions properties.");
                instance_extension_properties.resize(extension_count);
                VkResult enumerate_instance_extensions_properties_status = vkEnumerateInstanceExtensionProperties(nullptr, & extension_count, instance_extension_properties.data());
                ANDROMEDA_CORE_ASSERT(enumerate_instance_extensions_properties_status == VK_SUCCESS, "Failed to enumerate Vulkan extension properties.");
                switch(enumerate_instance_extensions_properties_status) {
                case VK_SUCCESS:
                    ANDROMEDA_CORE_INFO("Successfully enumerated extension properties.");
                    break;
                case VK_INCOMPLETE:
                    ANDROMEDA_CORE_INFO("Failed to enumerate extension properties. Provided extension_count {0} was invalid.", extension_count);
                    break;
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    ANDROMEDA_CORE_INFO("Failed to enumerate extension properties. Host out of memory.");
                    break;
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    ANDROMEDA_CORE_INFO("Failed to enumerate extension properties. Device out of memory.");
                    break;
                default:
                    ANDROMEDA_CORE_INFO("Unhandled enumerate extension result: {0}.", enumerate_instance_extensions_properties_status);
                    break;
                }
                return enumerate_instance_extensions_properties_status;
            }

            VkResult API::enumerate_instance_layer_properties(std::vector<VkLayerProperties> & available_validation_layers) {
                unsigned int layer_count;
                VkResult count_instance_layer_properties_status = vkEnumerateInstanceLayerProperties(& layer_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_instance_layer_properties_status == VK_SUCCESS, "Failed to count Vulkan validation layers.");
                available_validation_layers.resize(layer_count);
                VkResult enumerate_instance_layer_properties_status = vkEnumerateInstanceLayerProperties(& layer_count, available_validation_layers.data());
                switch(enumerate_instance_layer_properties_status) {
                case VK_SUCCESS:
                    ANDROMEDA_CORE_INFO("Successfully enumerated validation layer properties.");
                    break;
                case VK_INCOMPLETE:
                    ANDROMEDA_CORE_INFO("Failed to enumerate validation layer properties. Provided layer_count {0} was invalid.", layer_count);
                    break;
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    ANDROMEDA_CORE_INFO("Failed to enumerate validation layer properties. Host out of memory.");
                    break;
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    ANDROMEDA_CORE_INFO("Failed to enumerate validation layer properties. Device out of memory.");
                    break;
                default:
                    ANDROMEDA_CORE_INFO("Unhandled enumerate validation layer result: {0}.", enumerate_instance_layer_properties_status);
                    break;
                }
                return enumerate_instance_layer_properties_status;
            }

            bool API::check_desired_validation_layer_support(const std::vector<const char *> & desired_validation_layers) {
                std::vector<VkLayerProperties> available_validation_layers;
                VkResult enumerated = enumerate_instance_layer_properties(available_validation_layers);
                ANDROMEDA_CORE_ASSERT(enumerated == VK_SUCCESS, "Failed to enumate Vulkan validation layer properties.");

                ANDROMEDA_CORE_INFO("Available Vulkan Validation Layers: ");
                for (auto & available_validation_layer : available_validation_layers) ANDROMEDA_CORE_TRACE("{0}", available_validation_layer.layerName);

                auto result = std::all_of(desired_validation_layers.begin(), desired_validation_layers.end(), [available_validation_layers](const char * desired_validation_layer) {
                    auto result = std::find_if(available_validation_layers.begin(), available_validation_layers.end(), [desired_validation_layer](const VkLayerProperties & vkLayer) {
                        return strcmp(desired_validation_layer, vkLayer.layerName) == 0;
                    });
                    if(result == available_validation_layers.end()) {
                        ANDROMEDA_CORE_ERROR("The {0} validation layer is not available.", desired_validation_layer);
                        return false;
                    }
                    return true;
                });
                return result;
            }
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
