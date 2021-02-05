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
                virtual ~Context() override;

                virtual void initialize(std::any instance) override;
                virtual void shutdown() override;

                std::any get_native_context() const override {
                    return m_Surface;
                }

                std::pair<unsigned int, unsigned int> get_context_extent() const override {
                    return { m_Window->get_width(), m_Window->get_height() };
                }

              private:
                std::shared_ptr<Window> m_Window;
                VkInstance m_Instance;
                VkSurfaceKHR m_Surface;
            };
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
