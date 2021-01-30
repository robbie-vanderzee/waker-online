#include "context.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            Context::Context(GLFWWindow * window) : m_Window(window) {
                ANDROMEDA_CORE_ASSERT(m_Window, "Invalid window.");
            }
            void Context::initialize() {
                glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            }
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
