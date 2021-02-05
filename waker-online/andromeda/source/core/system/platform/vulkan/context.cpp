#include "context.hpp"

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            Context::Context(std::shared_ptr<Window> window) : m_Window(window) {
                ANDROMEDA_CORE_ASSERT(m_Window, "Invalid window.");
            }

            Context::~Context() {
            }

            void Context::initialize(std::any instance) {
                ANDROMEDA_CORE_INFO("Initializing Vulkan Context.");
                m_Instance = std::any_cast<VkInstance &> (instance);
                ANDROMEDA_CORE_ASSERT(m_Instance != VK_NULL_HANDLE, "Invalid Vulkan instance.");
                VkResult glfw_create_window_surface_status = glfwCreateWindowSurface(m_Instance, std::any_cast<GLFWwindow *> (m_Window->get_native_window()), nullptr, & m_Surface);
                ANDROMEDA_CORE_ASSERT(glfw_create_window_surface_status == VK_SUCCESS, "Failed to create Vulkan window surface context.");
            }

            void Context::shutdown() {
                ANDROMEDA_CORE_INFO("Destroying Vulkan Context.");
                vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
            }
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
