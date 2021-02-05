#include "api.hpp"
#include "core/instance.hpp"

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            void API::initialize() {
                ANDROMEDA_CORE_INFO("Initializing Vulkan API.");
                generate_vulkan_instance();
            }

            void API::process() {
                select_physical_device();
                create_logical_device();
                create_swap_chain();
            }

            void API::shutdown() {
                ANDROMEDA_CORE_INFO("Terminating Vulkan API.");
                vkDestroySwapchainKHR(m_API_Instance.logical_device, m_API_Instance.swap_chain, nullptr);
                if (m_Context) m_Context->shutdown();
                vkDestroyDevice(m_API_Instance.logical_device, nullptr);
                vkDestroyInstance(m_API_Instance.instance, nullptr);
            }

            void API::set_window_context(std::shared_ptr<Window> window) {
                m_Context = Graphics::Context::create_context(window);
                m_Context->initialize(m_API_Instance.instance);
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
                std::multimap<unsigned int, VkPhysicalDevice> sorted_physical_devices;
                for (auto physical_device : physical_devices) {
                    sorted_physical_devices.emplace(evaluate_physical_device(physical_device), physical_device);
                }
                auto best_physical_device = sorted_physical_devices.begin();
                m_API_Instance.physical_device = best_physical_device->second;
                ANDROMEDA_CORE_ASSERT(best_physical_device->first > 0, "No physical device sufficiently supports required Vulkan features.");
                vkGetPhysicalDeviceProperties(m_API_Instance.physical_device, & m_API_Instance.physical_device_properties);
                vkGetPhysicalDeviceFeatures(m_API_Instance.physical_device, & m_API_Instance.physical_device_features);
                get_queue_family_properties(m_API_Instance.queue_family_properties);
                verify_queue_family_properties();
                if (m_Context) {
                    VkSurfaceKHR surface = std::any_cast<VkSurfaceKHR> (m_Context->get_native_context());
                    auto result = std::find_if(m_API_Instance.queue_family_properties.begin(), m_API_Instance.queue_family_properties.end(), [this, surface](const auto &, int index = 0) {
                        VkBool32 supported = VK_FALSE;
                        auto get_physical_device_surface_support_KHR_status = get_physical_device_surface_support_KHR(m_API_Instance.physical_device, index++, surface, & supported);
                        ANDROMEDA_CORE_ASSERT(get_physical_device_surface_support_KHR_status == VK_SUCCESS, "Failed to get physical device surface support KHR");
                        return supported == VK_TRUE;
                    });
                    ANDROMEDA_CORE_ASSERT(result != m_API_Instance.queue_family_properties.end(), "Failed to find a present capable queue!");
                    m_API_Instance.present_queue_index = std::distance(result, m_API_Instance.queue_family_properties.begin());
                } else {
                    ANDROMEDA_CORE_INFO("No present queue selected.");
                }
                bool suitable = verify_device_is_suitable();
                ANDROMEDA_CORE_ASSERT(suitable, "Device is not suitable for use; lacking surface requirements.");
                ANDROMEDA_CORE_INFO("Selected Device: {0}", m_API_Instance.physical_device_properties.deviceName);
            }

            void API::create_logical_device() {
                float priority = 1.0f;
                create_device_queue_create_info(priority);
                create_device_create_info();
                m_API_Instance.logical_device_status = create_device();
                ANDROMEDA_CORE_ASSERT(m_API_Instance.logical_device_status == VK_SUCCESS, "Failed to create Vulkan logical device.");
                vkGetDeviceQueue(m_API_Instance.logical_device, m_API_Instance.graphics_queue_index.value(), 0, & m_API_Instance.graphics_queue);
                vkGetDeviceQueue(m_API_Instance.logical_device, m_API_Instance.present_queue_index.value(), 0, & m_API_Instance.present_queue);
            }

            void API::create_swap_chain() {
                Surface_Capabilities swap_chain_support = get_physical_device_capabilities(m_API_Instance.physical_device);
                VkSurfaceFormatKHR surface_format = select_swap_surface_format(swap_chain_support.formats);
                VkPresentModeKHR present_mode = select_swap_present_mode(swap_chain_support.present_modes);
                VkExtent2D extent = select_swap_extent(swap_chain_support.capabilities);
                unsigned int image_count = swap_chain_support.capabilities.minImageCount + 1;
                image_count = std::min(image_count, swap_chain_support.capabilities.maxImageCount);

                VkSwapchainCreateInfoKHR create_swap_chain_create_info{};
                create_swap_chain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
                create_swap_chain_create_info.surface = std::any_cast<VkSurfaceKHR>(m_Context->get_native_context());
                create_swap_chain_create_info.minImageCount = image_count;
                create_swap_chain_create_info.imageFormat = surface_format.format;
                create_swap_chain_create_info.imageColorSpace = surface_format.colorSpace;
                create_swap_chain_create_info.imageExtent = extent;
                create_swap_chain_create_info.imageArrayLayers = 1;
                create_swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

                create_swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
                create_swap_chain_create_info.queueFamilyIndexCount = 1;
                create_swap_chain_create_info.pQueueFamilyIndices = nullptr;
                unsigned int queue_family_indices[] = {m_API_Instance.graphics_queue_index.value(), m_API_Instance.present_queue_index.value()};
                if (m_API_Instance.graphics_queue_index.value() != m_API_Instance.present_queue_index.value()) {
                    create_swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
                    create_swap_chain_create_info.queueFamilyIndexCount = 2;
                    create_swap_chain_create_info.pQueueFamilyIndices = queue_family_indices;
                }
                create_swap_chain_create_info.preTransform = swap_chain_support.capabilities.currentTransform;
                create_swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
                create_swap_chain_create_info.presentMode = present_mode;
                create_swap_chain_create_info.clipped = VK_TRUE;
                create_swap_chain_create_info.oldSwapchain = VK_NULL_HANDLE;

                m_API_Instance.create_swap_chain_create_info = create_swap_chain_create_info;
                VkResult create_swap_chain_status = create_vk_swap_chain();
                ANDROMEDA_CORE_ASSERT(create_swap_chain_status == VK_SUCCESS, "Failed to create swap chain.");
                VkResult get_swap_chain_images_status = get_swap_chain_images_KHR(m_API_Instance.swap_chain_images);
                ANDROMEDA_CORE_ASSERT(get_swap_chain_images_status == VK_SUCCESS, "Failed to get swap chain images.");
                m_API_Instance.swap_chain_extent = extent;
                m_API_Instance.swap_chain_image_format = surface_format.format;
            }

            void API::create_application_info() {
                m_API_Instance.application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
                m_API_Instance.application_info.pNext = nullptr;
                m_API_Instance.application_info.pApplicationName = Instance::get_instance()->get_instance_name().c_str();
                m_API_Instance.application_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
                m_API_Instance.application_info.pEngineName = ANDROMEDA;
                m_API_Instance.application_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
                m_API_Instance.application_info.apiVersion = VK_MAKE_VERSION(1, 2, 0);
            }

            void API::create_instance_create_info() {
                unsigned int glfw_instance_extension_count = 0;
                const char ** glfw_instance_extensions = glfwGetRequiredInstanceExtensions(& glfw_instance_extension_count);
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
                switch (create_instance_status) {
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

            VkResult API::create_device() {
                VkResult create_device_status = vkCreateDevice(m_API_Instance.physical_device, &m_API_Instance.device_create_info, nullptr, &m_API_Instance.logical_device);
                switch (create_device_status) {
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

            VkResult API::create_vk_swap_chain() {
                VkResult create_swap_chain_status = vkCreateSwapchainKHR(m_API_Instance.logical_device, &m_API_Instance.create_swap_chain_create_info, nullptr, &m_API_Instance.swap_chain);
                switch (create_swap_chain_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully created Vulkan swap chain.");
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Device out of memory.");
                        break;
                    case VK_ERROR_DEVICE_LOST:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Device lost.");
                        break;
                    case VK_ERROR_SURFACE_LOST_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Surface lost.");
                        break;
                    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Native window in use.");
                        break;
                    case VK_ERROR_INITIALIZATION_FAILED:
                        ANDROMEDA_CORE_ERROR("Failed to create Vulkan swap chain. Initialization failed.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled Vulkan create device result: {0}.", create_swap_chain_status);
                        break;
                }
                return create_swap_chain_status;
            }

            VkResult API::enumerate_instance_extension_properties(std::vector<VkExtensionProperties> & instance_extension_properties) {
                unsigned int instance_extension_properties_count;
                VkResult count_instance_extension_properties_status = vkEnumerateInstanceExtensionProperties(nullptr, & instance_extension_properties_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_instance_extension_properties_status == VK_SUCCESS, "Failed to count Vulkan extensions properties.");
                instance_extension_properties.resize(instance_extension_properties_count);
                VkResult enumerate_instance_extensions_properties_status = vkEnumerateInstanceExtensionProperties(nullptr, & instance_extension_properties_count, instance_extension_properties.data());
                ANDROMEDA_CORE_ASSERT(enumerate_instance_extensions_properties_status == VK_SUCCESS, "Failed to enumerate Vulkan extension properties.");
                switch (enumerate_instance_extensions_properties_status) {
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
                switch (enumerate_instance_layer_properties_status) {
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

            VkResult API::enumerate_physical_device_extension_properties(std::vector<VkExtensionProperties> & device_extension_properties) {
                unsigned int device_extension_properties_count;
                VkResult count_device_extension_properties_status = vkEnumerateDeviceExtensionProperties(m_API_Instance.physical_device, nullptr, & device_extension_properties_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_device_extension_properties_status == VK_SUCCESS, "Failed to count Vulkan device extensions properties.");
                device_extension_properties.resize(device_extension_properties_count);
                VkResult enumerate_device_extensions_properties_status = vkEnumerateDeviceExtensionProperties(m_API_Instance.physical_device, nullptr, & device_extension_properties_count, device_extension_properties.data());
                ANDROMEDA_CORE_ASSERT(enumerate_device_extensions_properties_status == VK_SUCCESS, "Failed to enumerate Vulkan extension properties.");
                switch (enumerate_device_extensions_properties_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully enumerated extension properties.");
                        break;
                    case VK_INCOMPLETE:
                        ANDROMEDA_CORE_WARN("Failed to enumerate extension properties. Provided device_extension_properties_count {0} was invalid.", device_extension_properties_count);
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate extension properties. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate extension properties. Device out of memory.");
                        break;
                    case VK_ERROR_LAYER_NOT_PRESENT:
                        ANDROMEDA_CORE_ERROR("Failed to enumerate exxtension properties. Layer not present.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled enumerate extension result: {0}.", enumerate_device_extensions_properties_status);
                        break;
                }
                return enumerate_device_extensions_properties_status;
            }

            VkResult API::enumerate_physical_devices(std::vector<VkPhysicalDevice> & physical_devices) {
                unsigned int physical_device_count = 0;
                VkResult count_physical_devices_status = vkEnumeratePhysicalDevices(m_API_Instance.instance, & physical_device_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_physical_devices_status == VK_SUCCESS, "Failed to count Vulkan physical devices.");
                ANDROMEDA_CORE_ASSERT(physical_device_count > 0, "No physical devices found.");
                physical_devices.resize(physical_device_count);
                VkResult enumerate_physical_devices_status = vkEnumeratePhysicalDevices(m_API_Instance.instance, & physical_device_count, physical_devices.data());
                switch (enumerate_physical_devices_status) {
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

            VkResult API::get_physical_device_surface_support_KHR(VkPhysicalDevice device, unsigned int queue_family_index, VkSurfaceKHR surface, VkBool32 * supported) {
                VkResult get_physical_device_surface_support_KHR_status = vkGetPhysicalDeviceSurfaceSupportKHR(device, queue_family_index, surface, supported);
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_support_KHR_status == VK_SUCCESS, "Failed to determine device surface support.");
                switch (get_physical_device_surface_support_KHR_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully determined physical device surface support.");
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface support. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface support. Device out of memory.");
                        break;
                    case VK_ERROR_SURFACE_LOST_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface support. Surface lost.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled determine physical device surface support result: {0}.", get_physical_device_surface_support_KHR_status);
                        break;
                }
                return get_physical_device_surface_support_KHR_status;
            }

            VkResult API::get_physical_device_surface_capabilities_KHR(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR & capabilities) {
                VkResult get_physical_device_surface_capabilities_KHR_status = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, & capabilities);
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_capabilities_KHR_status == VK_SUCCESS, "Failed to determine device surface capabilities.");
                switch (get_physical_device_surface_capabilities_KHR_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully determined physical device surface capabilities.");
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface capabilities. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface capabilities. Device out of memory.");
                        break;
                    case VK_ERROR_SURFACE_LOST_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to determine physical device surface capabilities. Surface lost.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled determine physical device surface support result: {0}.", get_physical_device_surface_capabilities_KHR_status);
                        break;
                }
                return get_physical_device_surface_capabilities_KHR_status;
            }

            VkResult API::get_physical_device_surface_formats_KHR(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR> & formats) {
                unsigned int format_count = 0;
                VkResult count_physical_device_surface_formats_KHR_status = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, & format_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_physical_device_surface_formats_KHR_status == VK_SUCCESS, "Failed to count physical device surface formats.");
                formats.resize(format_count);
                VkResult get_physical_device_surface_formats_KHR_status = vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, & format_count, formats.data());
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_formats_KHR_status == VK_SUCCESS, "Failed to get physical device surface formats.");
                switch (get_physical_device_surface_formats_KHR_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully determined physical device surface formats.");
                        break;
                    case VK_INCOMPLETE:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface formats. Invalid format_count {0}.", format_count);
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface formats. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface formats. Device out of memory.");
                        break;
                    case VK_ERROR_SURFACE_LOST_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface formats. Surface lost.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled get physical device surface format result: {0}.", get_physical_device_surface_formats_KHR_status);
                        break;
                }
                return get_physical_device_surface_formats_KHR_status;
            }

            VkResult API::get_physical_device_surface_present_modes_KHR(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR> & present_modes) {
                unsigned int present_mode_count = 0;
                VkResult count_physical_device_surface_present_modes_KHR_status = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, & present_mode_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_physical_device_surface_present_modes_KHR_status == VK_SUCCESS, "Failed to count physical device surface present modes.");
                present_modes.resize(present_mode_count);
                VkResult get_physical_device_surface_present_modes_KHR_status = vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, & present_mode_count, present_modes.data());
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_present_modes_KHR_status == VK_SUCCESS, "Failed to get physical device surface present modes.");
                switch (get_physical_device_surface_present_modes_KHR_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully determined physical device surface present modes.");
                        break;
                    case VK_INCOMPLETE:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface present modes. Invalid format_count {0}.", present_mode_count);
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface present modes. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface present modes. Device out of memory.");
                        break;
                    case VK_ERROR_SURFACE_LOST_KHR:
                        ANDROMEDA_CORE_ERROR("Failed to get physical device surface present modes. Surface lost.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled get physical device surface present modes result: {0}.", get_physical_device_surface_present_modes_KHR_status);
                        break;
                }
                return get_physical_device_surface_present_modes_KHR_status;
            }

            VkResult API::get_swap_chain_images_KHR(std::vector<VkImage> & images) {
                unsigned int image_count = 0;
                VkResult count_swap_chain_images_status = vkGetSwapchainImagesKHR(m_API_Instance.logical_device, m_API_Instance.swap_chain, &image_count, nullptr);
                ANDROMEDA_CORE_ASSERT(count_swap_chain_images_status == VK_SUCCESS, "Failed to count swap chain images.");
                images.resize(image_count);
                VkResult get_swap_chain_images_status = vkGetSwapchainImagesKHR(m_API_Instance.logical_device, m_API_Instance.swap_chain, &image_count, images.data());
                switch (get_swap_chain_images_status) {
                    case VK_SUCCESS:
                        ANDROMEDA_CORE_INFO("Successfully obtained swap chain images.");
                        break;
                    case VK_INCOMPLETE:
                        ANDROMEDA_CORE_ERROR("Failed to get swap chain images. Invalid image_count {0}.", image_count);
                        break;
                    case VK_ERROR_OUT_OF_HOST_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get swap chain images. Host out of memory.");
                        break;
                    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
                        ANDROMEDA_CORE_ERROR("Failed to get swap chain images. Device out of memory.");
                        break;
                    default:
                        ANDROMEDA_CORE_CRITICAL("Unhandled get swap chain images result: {0}.", get_swap_chain_images_status);
                        break;
                }
                return get_swap_chain_images_status;
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
                if (!physical_device_features.geometryShader) return 0;
                score += physical_device_properties.limits.maxImageDimension3D;
                return score;
            }

            void API::verify_queue_family_properties() {
                auto result = std::find_if(m_API_Instance.queue_family_properties.begin(), m_API_Instance.queue_family_properties.end(), [](const auto & queue_family_property) {
                    return queue_family_property.queueFlags & VK_QUEUE_GRAPHICS_BIT;
                });
                ANDROMEDA_CORE_ASSERT(result != m_API_Instance.queue_family_properties.end(), "Failed to find necessary queue families in physical device");
                m_API_Instance.graphics_queue_index = std::distance(result, m_API_Instance.queue_family_properties.begin());
            }

            Surface_Capabilities API::get_physical_device_capabilities(VkPhysicalDevice & device) {
                Surface_Capabilities capabilities;
                VkSurfaceKHR surface = std::any_cast<VkSurfaceKHR> (m_Context->get_native_context());
                auto get_physical_device_surface_capabilities_KHR_status = get_physical_device_surface_capabilities_KHR(device, surface, capabilities.capabilities);
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_capabilities_KHR_status == VK_SUCCESS, "Failed to find device surface capabilities.");
                auto get_physical_device_surface_formats_KHR_status = get_physical_device_surface_formats_KHR(device, surface, capabilities.formats);
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_formats_KHR_status == VK_SUCCESS, "Failed to get device surface formats.");
                auto get_physical_device_surface_present_modes_KHR_status = get_physical_device_surface_present_modes_KHR(device, surface, capabilities.present_modes);
                ANDROMEDA_CORE_ASSERT(get_physical_device_surface_present_modes_KHR_status == VK_SUCCESS, "Failed to get device surface present modes.");
                return capabilities;
            }

            VkSurfaceFormatKHR API::select_swap_surface_format(const std::vector<VkSurfaceFormatKHR> & available_formats) {
                auto result = std::find_if(available_formats.begin(), available_formats.end(), [](const auto & available_format) {
                    return available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
                });
                return *result;
            }

            VkPresentModeKHR API::select_swap_present_mode(const std::vector<VkPresentModeKHR> & available_present_modes) {
                auto result = std::find_if(available_present_modes.begin(), available_present_modes.end(), [](const auto & available_present_mode) {
                    return available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR;
                });
                return result != available_present_modes.end() ? *result : VK_PRESENT_MODE_FIFO_KHR;
            }

            VkExtent2D API::select_swap_extent(const VkSurfaceCapabilitiesKHR & capabilities) {
                auto [width, height] = m_Context->get_context_extent();
                VkExtent2D extent = { width, height };
                extent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, extent.width));
                extent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, extent.height));
                return extent;
            }

            void API::create_device_queue_create_info(float & queue_priorities) {
                std::set<unsigned int> queues = {m_API_Instance.graphics_queue_index.value(), m_API_Instance.present_queue_index.value()};
                for (auto index : queues) {
                    VkDeviceQueueCreateInfo device_queue_create_info;
                    device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                    device_queue_create_info.pNext = nullptr;
                    device_queue_create_info.flags = 0;
                    device_queue_create_info.queueFamilyIndex = index;
                    device_queue_create_info.queueCount = 1;
                    device_queue_create_info.pQueuePriorities = & queue_priorities;
                    m_API_Instance.device_queue_create_infos.push_back(device_queue_create_info);
                }
            }

            void API::create_device_create_info() {
                m_API_Instance.device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
                m_API_Instance.device_create_info.pNext = nullptr;
                m_API_Instance.device_create_info.flags = 0;
                m_API_Instance.device_create_info.queueCreateInfoCount = m_API_Instance.device_queue_create_infos.size();
                m_API_Instance.device_create_info.pQueueCreateInfos = m_API_Instance.device_queue_create_infos.data();
                m_API_Instance.device_create_info.enabledLayerCount = 0;
                m_API_Instance.device_create_info.ppEnabledLayerNames = nullptr;
                m_API_Instance.device_create_info.enabledExtensionCount = desired_device_extensions.size();
                m_API_Instance.device_create_info.ppEnabledExtensionNames = desired_device_extensions.data();
                m_API_Instance.device_create_info.pEnabledFeatures = nullptr;
#ifdef DEBUG
                m_API_Instance.device_create_info.enabledLayerCount = m_API_Instance.instance_create_info.enabledLayerCount;
                m_API_Instance.device_create_info.ppEnabledLayerNames = m_API_Instance.instance_create_info.ppEnabledLayerNames;
#endif
            }
            bool API::verify_device_is_suitable() {
                bool extensions_supported = verify_desired_extension_support(desired_device_extensions);
                auto capabilities = get_physical_device_capabilities(m_API_Instance.physical_device);
                return m_API_Instance.present_queue_index.has_value() && m_API_Instance.graphics_queue_index.has_value() && extensions_supported && !capabilities.formats.empty() && !capabilities.present_modes.empty();
            }

            bool API::verify_desired_extension_support(const std::vector<const char *> & desired_device_extensions) {
                std::vector<VkExtensionProperties> available_extensions;
                auto enumerate_instance_extension_properties_status = enumerate_physical_device_extension_properties(available_extensions);
                ANDROMEDA_CORE_ASSERT(enumerate_instance_extension_properties_status == VK_SUCCESS, "Failed to enumerate instance extension properties.");
                for (auto & available_extension : available_extensions) ANDROMEDA_CORE_TRACE("{0}", available_extension.extensionName);
                auto result = std::all_of(desired_device_extensions.begin(), desired_device_extensions.end(), [available_extensions](const char * desired_extension) {
                    auto result = std::find_if(available_extensions.begin(), available_extensions.end(), [desired_extension](const VkExtensionProperties & vkExtension) {
                        return strcmp(desired_extension, vkExtension.extensionName) == 0;
                    });
                    if (result == available_extensions.end()) {
                        ANDROMEDA_CORE_ERROR("The {0} extension is not available.", desired_extension);
                        return false;
                    }
                    return true;
                });
                return result;
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
                    if (result == available_validation_layers.end()) {
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
