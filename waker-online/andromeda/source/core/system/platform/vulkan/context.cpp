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
                ANDROMEDA_CORE_INFO("Initializing Vulkan Context.");
            }
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
