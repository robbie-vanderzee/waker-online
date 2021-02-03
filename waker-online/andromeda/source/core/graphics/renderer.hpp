#pragma once

#include "command.hpp"

namespace Andromeda {
    namespace Graphics {
        namespace Renderer {
            void initialize();
            void process();
            void shutdown();

            inline API_TYPE get_API() {
                return API::get_API();
            }
        };
    } /* Graphics */
} /* Andromeda */
