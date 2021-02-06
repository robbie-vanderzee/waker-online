#pragma once

#include "api.hpp"

#include "core/system/interface/window/window.hpp"

namespace Andromeda {
    namespace Graphics {
        class Renderer {
          public:
            Renderer(Graphics::Info info, API::Type type);
            ~Renderer();
            void initialize();
            void process();
            void shutdown();

            inline API::Type get_API_Type() {
                return m_API->get_API_Type();
            }

            void set_window_context(std::weak_ptr<Window> window) {
                m_API->set_window_context(window);
            }
          private:
            std::unique_ptr<API> m_API;
            Info m_Info;
        };
    } /* Graphics */
} /* Andromeda */
