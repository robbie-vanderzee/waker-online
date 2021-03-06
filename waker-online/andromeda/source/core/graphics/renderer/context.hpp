#pragma once

#include "core/core.hpp"

#include "renderer.hpp"

#include "core/system/interface/display/window.hpp"

namespace Andromeda {
    namespace Graphics {
        class Renderer::Context {
            friend class Window;
          public:
            using Extent = Window::Viewport;
          public:
            virtual ~Context() = default;

            virtual void initialize(std::any instance) = 0;
            virtual void shutdown() = 0;

            virtual std::any get_native_context() const = 0;
            virtual Extent get_context_extent() const = 0;

            static std::unique_ptr<Context> create_context(API::Type type, std::shared_ptr<Window> window);
        };
    } /* Graphics */
} /* Andromeda */
