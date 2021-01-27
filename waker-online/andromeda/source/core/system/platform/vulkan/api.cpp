#include "api.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            void API::initialize() {
                ANDROMEDA_CORE_INFO("Initializing Vulkan API.");
                generate_instance();
            }
            void API::process() {
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
                m_InstanceCreateInfo.pApplicationInfo = &m_ApplicationInfo;

                m_GlfwExtensionCount = 0;
                m_GlfwExtensions = glfwGetRequiredInstanceExtensions(&m_GlfwExtensionCount);
                m_InstanceCreateInfo.enabledExtensionCount  = m_GlfwExtensionCount;
                m_InstanceCreateInfo.ppEnabledExtensionNames = m_GlfwExtensions;
                m_InstanceCreateInfo.enabledLayerCount = 0;

                ANDROMEDA_CORE_ASSERT(vkCreateInstance(&m_InstanceCreateInfo, nullptr, &m_Instance) == VK_SUCCESS, "Failed to create Vulkan API instance.");

                ANDROMEDA_CORE_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &m_ExtensionCount, nullptr) == VK_SUCCESS, "Failed to count Vulkan extensions properties.");
                m_Extensions.resize(m_ExtensionCount);
                ANDROMEDA_CORE_ASSERT(vkEnumerateInstanceExtensionProperties(nullptr, &m_ExtensionCount, m_Extensions.data()) == VK_SUCCESS, "Failed to enumerate Vulkan extension properties.");

                ANDROMEDA_CORE_INFO("Vulkan Extensions: ");
                for (auto &extension : m_Extensions) ANDROMEDA_CORE_TRACE("{0}", extension.extensionName);

            }
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
