#pragma once

#include "command.hpp"

namespace Andromeda {
    namespace Graphics {
        class Renderer {
          public:
            static void initialize();
            static void shutdown();

            inline static API_TYPE get_API() {
                return API::get_API();
            }
        };
    } /* Graphics */
} /* Andromeda */
