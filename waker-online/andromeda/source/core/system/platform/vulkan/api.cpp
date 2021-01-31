#include "api.hpp"
#include "core/instance.hpp"

#include <GLFW/glfw3.h>

#include <cstring>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            void API::initialize() {
                ANDROMEDA_CORE_INFO("Initializing Vulkan API.");
                generate_vulkan_instance();
                select_physical_device();
                create_logical_device();
            }
            void API::shutdown() {
                vkDestroyDevice(m_API_Instance.logical_device, nullptr);
                vkDestroyInstance(m_API_Instance.instance, nullptr);
            }

            void API::generate_vulkan_instance() {
                create_application_info();
                create_instance_create_info();
                m_API_Instance.instance_status = create_instance();
                ANDROMEDA_CORE_ASSERT(m_API_Instance.instance_status == VK_SUCCESS, "Failed to create Vulkan API instance.");
#ifdef DEBUG
                std::vector<VkExtensionProperties> instance_extension_properties;
                VkResult enumerate_instance_extension_properties_status = enumerate_instance_extension_properties(instance_extension_properties);
                ANDROMEDA_CORE_ASSERT(enumerate_instance_extension_properties_status == VK_SUCCESS, "Failed to enumerate Vulkan extension layer properties.");

                ANDROMEDA_CORE_INFO("Vulkan Extensions: ");
                for (auto & instance_extension_property : instance_extension_properties) ANDROMEDA_CORE_TRACE("{0}", instance_extension_property.extensionName);
#endif
            }

            void API::select_physical_device() {
                m_API_Instance.physical_device = VK_NULL_HANDLE;
                std::vector<VkPhysicalDevice> physical_devices;
                VkResult enumerate_physical_devices_status = enumerate_physical_devices(physical_devices);
                ANDROMEDA_CORE_ASSERT(enumerate_physical_devices_status == VK_SUCCESS, "Failed to enumerate Vulkan physical devices.");
#ifdef DEBUG
                ANDROMEDA_CORE_INFO("Physical Devices: ");
                VkPhysicalDeviceProperties physical_device_properties;
                for (auto & physical_device : physical_devices) {
                    vkGetPhysicalDeviceProperties(physical_device, & physical_device_properties);
                    ANDROMEDA_CORE_TRACE("{0}", physical_device_properties.deviceName);
                }
#endif
                std::map<unsigned int, VkPhysicalDevice> sorted_physical_devices;
                for( auto physical_device : physical_devices) {
                    sorted_physical_devices.emplace(evaluate_physical_device(physical_device), physical_device);
                }
                auto best_physical_device = sorted_physical_devices.begin();
                m_API_Instance.physical_device = best_physical_device->second;
                ANDROMEDA_CORE_ASSERT(best_physical_device->first > 0, "No physical device sufficiently supports required Vulkan features.");
                vkGetPhysicalDeviceProperties(m_API_Instance.physical_device, & m_API_Instance.physical_device_properties);
                vkGetPhysicalDeviceFeatures(m_API_Instance.physical_device, & m_API_Instance.physical_device_features);
                get_queue_family_properties(m_API_Instance.queue_family_properties);
                verify_queue_family_properties();
                ANDROMEDA_CORE_INFO("Selected Device: {0}", m_API_Instance.physical_device_properties.deviceName);
            }

            void API::create_logical_device() {
                float priority = 1.0f;
                create_device_queue_create_info(priority);
                create_device_create_info();
                m_API_Instance.logical_device_status = create_device();
                ANDROMEDA_CORE_ASSERT(m_API_Instance.logical_device_status == VK_SUCCESS, "Failed to create Vulkan logical device.");
                vkGetDeviceQueue(m_API_Instance.logical_device, 0, 0, & m_API_Instance.graphics_queue);
            }

            void API::create_application_info() {
                m_API_Instance.application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                m_API_Instance.application_info.pNext = nullptr;
                m_API_Instance.application_info.pApplicationName = Instance::get_instance().get_instance_name().c_str();
                m_API_Instance.application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
                m_API_Instance.application_info.pEngineName = ANDROMEDA;
                m_API_Instance.application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
                m_API_Instance.application_info.apiVersion = VK_MAKE_VERSION(1, 2, 0);
            }

            void API::create_instance_create_info() {
                unsigned int glfw_instance_extension_count = 0;
                const char ** glfw_instance_extensions = glfwGetRequiredInstanceExtensions( & glfw_instance_extension_count);
                m_API_Instance.instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
                m_API_Instance.instance_create_info.pNext = nullptr;
                m_API_Instance.instance_create_info.flags = 0;
                m_API_Instance.instance_create_info.pApplicationInfo = & m_API_Instance.application_info;
                m_API_Instance.instance_create_info.enabledLayerCount = 0;
                m_API_Instance.instance_create_info.ppEnabledLayerNames = nullptr;
                m_API_Instance.instance_create_info.enabledExtensionCount  = glfw_instance_extension_count;
                m_API_Instance.instance_create_info.ppEnabledExtensionNames = glfw_instance_extensions;

#ifdef DEBUG
                bool verify_desired_validation_layer_support_status = verify_desired_validation_layer_support(desired_validation_layers);
                ANDROMEDA_CORE_ASSERT(verify_desired_validation_layer_support_status, "Failed to apply all Vulkan validation layers.");

                m_API_Instance.instance_create_info.enabledLayerCount = desired_validation_layers.size();
                m_API_Instance.instance_create_info.ppEnabledLayerNames = desired_validation_layers.data();

                ANDROMEDA_CORE_INFO("Enabled Vulkan Validation Layers: ");
                for (auto & enabled_validation_layer : desired_validation_layers) ANDROMEDA_CORE_TRACE("{0}", enabled_validation_layer);
#endif
            }

            VkResult API::create_instance() {
                VkResult create_instance_status = vkCreateInstance(& m_API_Instance.instance_create_info, nullptr, & m_API_Instance.instance);
                switch(create_instance_status) {
                case VK_SUCCESS:
                    ANDROMEDA_CORE_INFO("Successfully initialized Vulkan API Instance.");
                    break;
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Host out of memory.");
                    break;
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Device out of memory.");
                    break;
                case VK_ERROR_INITIALIZATION_FAILED:
                    ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Initialization failed.");
                    break;
                case VK_ERROR_LAYER_NOT_PRESENT:
                    ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Validation Layers not present.");
                    break;
                case VK_ERROR_EXTENSION_NOT_PRESENT:
                    ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Extensions not present.");
                    break;
                case VK_ERROR_INCOMPATIBLE_DRIVER:
                    ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Instance. Incompatible Driver.");
                    break;
                default:
                    ANDROMEDA_CORE_CRITICAL("Unhandled Vulkan create instance result: {0}.", create_instance_status);
                    break;
                }
                return create_instance_status;
            }

            VkResult API::create_device(){
                VkResult create_device_status = vkCreateDevice(m_API_Instance.physical_device, &m_API_Instance.device_create_info, nullptr, &m_API_Instance.logical_device);
                switch(create_device_status){
                  case VK_SUCCESS:
                      ANDROMEDA_CORE_INFO("Successfully initialized Vulkan API Device.");
                      break;
                  case VK_ERROR_OUT_OF_HOST_MEMORY:
                      ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device Host out of memory.");
                      break;
                  case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                      ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. Device out of memory.");
                      break;
                  case VK_ERROR_INITIALIZATION_FAILED:
                      ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. Initialization failed.");
                      break;
                  case VK_ERROR_EXTENSION_NOT_PRESENT:
                      ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. Extensions not present.");
                      break;
                  case VK_ERROR_FEATURE_NOT_PRESENT:
                      ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. Features not present.");
                      break;
                  case VK_ERROR_TOO_MANY_OBJECTS:
                      ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. Too many objects.");
                      break;
                  case VK_ERROR_DEVICE_LOST:
                      ANDROMEDA_CORE_ERROR("Failed to initialize Vulkan API Device. The device was lost.");
                      break;
                  default:
                      ANDROMEDA_CORE_CRITICAL("Unhandled Vulkan create device result: {0}.", create_device_status);
                      break;
                }
                return create_device_status;
            }


            VkResult API::enumerate_instance_extension_properties(std::vector<VkExtensionProperties> & instance_extension_properties) {
                unsigned int instance_extension_properties_count;
                VkResult count_instance_extension_properties_status = vkEnumerateInstanceExtensionProperties(nullptr, & instance_extension_properties_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_instance_extension_properties_status == VK_SUCCESS, "Failed to count Vulkan extensions properties.");
                instance_extension_properties.resize(instance_extension_properties_count);
                VkResult enumerate_instance_extensions_properties_status = vkEnumerateInstanceExtensionProperties(nullptr, & instance_extension_properties_count, instance_extension_properties.data());
                ANDROMEDA_CORE_ASSERT(enumerate_instance_extensions_properties_status == VK_SUCCESS, "Failed to enumerate Vulkan extension properties.");
                switch(enumerate_instance_extensions_properties_status) {
                case VK_SUCCESS:
                    ANDROMEDA_CORE_INFO("Successfully enumerated extension properties.");
                    break;
                case VK_INCOMPLETE:
                    ANDROMEDA_CORE_WARN("Failed to enumerate extension properties. Provided instance_extension_properties_count {0} was invalid.", instance_extension_properties_count);
                    break;
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    ANDROMEDA_CORE_ERROR("Failed to enumerate extension properties. Host out of memory.");
                    break;
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    ANDROMEDA_CORE_ERROR("Failed to enumerate extension properties. Device out of memory.");
                    break;
                default:
                    ANDROMEDA_CORE_CRITICAL("Unhandled enumerate extension result: {0}.", enumerate_instance_extensions_properties_status);
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
                    ANDROMEDA_CORE_WARN("Failed to enumerate validation layer properties. Provided layer_count {0} was invalid.", layer_count);
                    break;
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    ANDROMEDA_CORE_ERROR("Failed to enumerate validation layer properties. Host out of memory.");
                    break;
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    ANDROMEDA_CORE_ERROR("Failed to enumerate validation layer properties. Device out of memory.");
                    break;
                default:
                    ANDROMEDA_CORE_CRITICAL("Unhandled enumerate validation layer result: {0}.", enumerate_instance_layer_properties_status);
                    break;
                }
                return enumerate_instance_layer_properties_status;
            }

            VkResult API::enumerate_physical_devices(std::vector<VkPhysicalDevice> & physical_devices) {
                unsigned int physical_device_count = 0;
                VkResult count_physical_devices_status = vkEnumeratePhysicalDevices(m_API_Instance.instance, & physical_device_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_physical_devices_status == VK_SUCCESS, "Failed to count Vulkan physical devices.");
                ANDROMEDA_CORE_ASSERT(physical_device_count > 0, "No physical devices found.");
                physical_devices.resize(physical_device_count);
                VkResult enumerate_physical_devices_status = vkEnumeratePhysicalDevices(m_API_Instance.instance, & physical_device_count, physical_devices.data());
                switch(enumerate_physical_devices_status) {
                case VK_SUCCESS:
                    ANDROMEDA_CORE_INFO("Successfully enumerated physical devices.");
                    break;
                case VK_INCOMPLETE:
                    ANDROMEDA_CORE_WARN("Failed to enumerate physical devices. Provided physical_device_count {0} was invalid.", physical_device_count);
                    break;
                case VK_ERROR_OUT_OF_HOST_MEMORY:
                    ANDROMEDA_CORE_ERROR("Failed to enumerate physical devices. Host out of memory.");
                    break;
                case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                    ANDROMEDA_CORE_ERROR("Failed to enumerate physical devices. Device out of memory.");
                    break;
                case VK_ERROR_INITIALIZATION_FAILED:
                    ANDROMEDA_CORE_ERROR("Failed to enumerate physical devices. Initialization failed.");
                    break;
                default:
                    ANDROMEDA_CORE_CRITICAL("Unhandled enumerate physical device result: {0}.", enumerate_physical_devices_status);
                    break;
                }
                return enumerate_physical_devices_status;
            }


            void API::get_queue_family_properties(std::vector<VkQueueFamilyProperties> & queue_family_properties) {
                unsigned int queue_family_properties_count = 0;
                vkGetPhysicalDeviceQueueFamilyProperties(m_API_Instance.physical_device, & queue_family_properties_count, nullptr);
                queue_family_properties.resize(queue_family_properties_count);
                vkGetPhysicalDeviceQueueFamilyProperties(m_API_Instance.physical_device, & queue_family_properties_count, queue_family_properties.data());
            }

            unsigned int API::evaluate_physical_device(const VkPhysicalDevice & physical_device) {
                int score = 0;
                VkPhysicalDeviceProperties physical_device_properties;
                VkPhysicalDeviceFeatures physical_device_features;
                vkGetPhysicalDeviceProperties(physical_device, & physical_device_properties);
                vkGetPhysicalDeviceFeatures(physical_device, & physical_device_features);

                if (physical_device_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) score++;
                if(!physical_device_features.geometryShader) return 0;
                score += physical_device_properties.limits.maxImageDimension3D;
                return score;
            }

            void API::verify_queue_family_properties() {
                auto result = std::find_if(m_API_Instance.queue_family_properties.begin(), m_API_Instance.queue_family_properties.end(), [](const auto & queue_family_property) {
                    return queue_family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT;
                });
                ANDROMEDA_CORE_ASSERT(result != m_API_Instance.queue_family_properties.end(), "Failed to find necessary queue families in physical device");
            }

            void API::create_device_queue_create_info(float & queue_priorities) {
                m_API_Instance.device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                m_API_Instance.device_queue_create_info.pNext = nullptr;
                m_API_Instance.device_queue_create_info.flags = 0;
                m_API_Instance.device_queue_create_info.queueFamilyIndex = 0;
                m_API_Instance.device_queue_create_info.queueCount = 1;
                m_API_Instance.device_queue_create_info.pQueuePriorities = &queue_priorities;
            }

            void API::create_device_create_info() {
                m_API_Instance.device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                m_API_Instance.device_create_info.pNext = nullptr;
                m_API_Instance.device_create_info.flags = 0;
                m_API_Instance.device_create_info.queueCreateInfoCount = 1;
                m_API_Instance.device_create_info.pQueueCreateInfos = & m_API_Instance.device_queue_create_info;
                m_API_Instance.device_create_info.enabledLayerCount = 0;
                m_API_Instance.device_create_info.ppEnabledLayerNames = nullptr;
                m_API_Instance.device_create_info.enabledExtensionCount = 0;
                m_API_Instance.device_create_info.ppEnabledExtensionNames = nullptr;
                m_API_Instance.device_create_info.pEnabledFeatures = & m_API_Instance.physical_device_features;
#ifdef DEBUG
                m_API_Instance.device_create_info.enabledLayerCount = m_API_Instance.instance_create_info.enabledLayerCount;
                m_API_Instance.device_create_info.ppEnabledLayerNames = m_API_Instance.instance_create_info.ppEnabledLayerNames;
#endif
            }

            bool API::verify_desired_validation_layer_support(const std::vector<const char *> & desired_validation_layers) {
                std::vector<VkLayerProperties> available_validation_layers;
                VkResult enumerate_instance_layer_properties_status = enumerate_instance_layer_properties(available_validation_layers);
                ANDROMEDA_CORE_ASSERT(enumerate_instance_layer_properties_status == VK_SUCCESS, "Failed to enumate Vulkan validation layer properties.");

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
