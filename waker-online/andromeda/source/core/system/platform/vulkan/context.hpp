#pragma once

#include "api.hpp"
#include "core/graphics/renderer/context.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            class Context : public Andromeda::Graphics::Renderer::Context {
              public:
                Context(std::shared_ptr<Window> window);
                ~Context() override;

                void initialize(std::any instance) override;
                void shutdown() override;

                inline std::any get_native_context() const override {
                    return m_Surface;
                }

                inline Extent get_context_extent() const override {
                    return m_Window->get_viewport();
                }

              private:
                std::shared_ptr<Window> m_Window;
                VkInstance m_Instance;
                VkSurfaceKHR m_Surface;
            };
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
