#pragma once

#include "core/graphics/context.hpp"

struct GLFWWindow;

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            class Context : public Andromeda::Graphics::Context {
              public:
                Context(GLFWWindow * window);

                virtual void initialize() override;
              private:
                GLFWWindow * m_Window;
            };
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
