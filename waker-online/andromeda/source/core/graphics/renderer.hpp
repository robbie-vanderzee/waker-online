#pragma once

#include "render.hpp"

namespace Andromeda {
    namespace Graphics {
        class Renderer {
          public:
            static void initialize();
            static void process();
            static void shutdown();

            static API_TYPE get_API() {
                return get_API();
            }
        };
    } /* Graphics */
} /* Andromeda */
