#pragma once

#include "api.hpp"

#include "core/system/interface/window/window.hpp"

namespace Andromeda {
    namespace Graphics {
        class Command {
          public:
            static void initialize() {
                s_API->initialize();
            }
            static void shutdown() {
                s_API->shutdown();
            }
            static void set_context(std::shared_ptr<Window> window) {
                s_API->set_context(window);
            }
          private:
            static std::unique_ptr<API> s_API;
        };
    } /* Graphics */
} /* Andromeda */