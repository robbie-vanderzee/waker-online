#pragma once

#include "core/graphics/api.hpp"
#include "core/graphics/context.hpp"
#include "core/system/interface/window/window.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            struct Surface_Capabilities {
                VkSurfaceCapabilitiesKHR capabilities;
                std::vector<VkSurfaceFormatKHR> formats;
                std::vector<VkPresentModeKHR> present_modes;
            };
            struct API_Instance {
                /* Instance */
                VkApplicationInfo application_info;
                VkInstanceCreateInfo instance_create_info;
                VkInstance instance;
                VkResult instance_status;
                /* Physical Device */
                VkPhysicalDevice physical_device;
                VkPhysicalDeviceProperties physical_device_properties;
                VkPhysicalDeviceFeatures physical_device_features;
                std::vector<VkQueueFamilyProperties> queue_family_properties;
                /* Logical Device */
                std::vector<VkDeviceQueueCreateInfo> device_queue_create_infos;
                std::optional<unsigned int> graphics_queue_index;
                std::optional<unsigned int> present_queue_index;
                VkDeviceCreateInfo device_create_info;
                VkDevice logical_device;
                VkResult logical_device_status;
                /* Pipeline */
                VkQueue graphics_queue;
                VkQueue present_queue;

            };
            class API : public Andromeda::Graphics::API {
              public:
                virtual void initialize() override;
                virtual void process() override;
                virtual void shutdown() override;

                virtual void set_window_context(std::shared_ptr<Window> window) override;

              private:
                void generate_vulkan_instance();
                void select_physical_device();
                void create_logical_device();

              private:
                void create_application_info();
                void create_instance_create_info();
                VkResult create_instance();

                VkResult enumerate_instance_extension_properties(std::vector<VkExtensionProperties> & instance_extension_properties);
                VkResult enumerate_instance_layer_properties(std::vector<VkLayerProperties> & instance_layer_properties);

                VkResult enumerate_physical_device_extension_properties(std::vector<VkExtensionProperties> & device_extension_properties);

              private:
                VkResult enumerate_physical_devices(std::vector<VkPhysicalDevice> & physical_devices);
                void get_queue_family_properties(std::vector<VkQueueFamilyProperties> & queue_family_properties);

                VkResult get_physical_device_surface_support_KHR(VkPhysicalDevice device, unsigned int queue_family_index, VkSurfaceKHR surface, VkBool32 * supported);
                VkResult get_physical_device_surface_capabilities_KHR(VkPhysicalDevice device, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR & capabilities);
                VkResult get_physical_device_surface_formats_KHR(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkSurfaceFormatKHR> & formats);
                VkResult get_physical_device_surface_present_modes_KHR(VkPhysicalDevice device, VkSurfaceKHR surface, std::vector<VkPresentModeKHR> & present_modes);

                unsigned int evaluate_physical_device(const VkPhysicalDevice & physical_device);
                void verify_queue_family_properties();

                Surface_Capabilities get_physical_device_capabilities(VkPhysicalDevice & device);

              private:
                void create_device_queue_create_info(float & queue_priorities);

                void create_device_create_info();
                VkResult create_device();
                bool verify_device_is_suitable();

              private:
                const std::vector<const char *> desired_device_extensions = {
                    VK_KHR_SWAPCHAIN_EXTENSION_NAME
                };
                bool verify_desired_extension_support(const std::vector<const char*> & desired_device_extensions);

              private:
                const std::vector<const char *> desired_validation_layers = { "VK_LAYER_KHRONOS_validation" };
                bool verify_desired_validation_layer_support(const std::vector<const char *> & desired_validation_layers);

              private:
                API_Instance m_API_Instance;
                std::unique_ptr<Graphics::Context> m_Context;
            };
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
