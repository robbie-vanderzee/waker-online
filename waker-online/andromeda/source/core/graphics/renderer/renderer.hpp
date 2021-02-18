#pragma once

#include "core/core.hpp"

#include "api.hpp"

#include "core/system/interface/display/window.hpp"

namespace Andromeda {
    namespace Graphics {
        class Renderer {
          public:
            class Context;
          public:
            static void initialize(Graphics::Info info, API::Type type);
            static void process();
            static void shutdown();

            static inline API::Type get_API_Type() {
                return s_API->get_API_Type();
            }

            static void set_window_context(std::shared_ptr<Window> window);

            static void present() {
                s_API->present();
            };
          private:
            static std::unique_ptr<API> s_API;
            static Info m_Info;
        };
    } /* Graphics */
} /* Andromeda */
