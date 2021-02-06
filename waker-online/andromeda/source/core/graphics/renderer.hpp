#pragma once

#include "command.hpp"

namespace Andromeda {
    namespace Graphics {
        namespace Renderer {
            void initialize();
            void process();
            void shutdown();

            inline API::Type get_API_Type() {
                return API::get_API_Type();
            }
        };
    } /* Graphics */
} /* Andromeda */
