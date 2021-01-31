#pragma once

#include "api.hpp"
#include "core/graphics/context.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            class Context : public Andromeda::Graphics::Context {
              public:
                Context(std::shared_ptr<Window> window);

                virtual void initialize(void * instance) override;
                virtual void shutdown() override;

              private:
                std::shared_ptr<Window> m_Window;
                VkInstance m_Instance;
                VkSurfaceKHR m_Surface;
            };
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
