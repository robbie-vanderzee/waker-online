#pragma once

#include "api.hpp"

namespace Andromeda {
    namespace Graphics {
        class Render {
          public:
            static void initialize() {
                s_API->initialize();
            }
          private:
            static std::unique_ptr<API> s_API;
        };
    } /* Graphics */
} /* Andromeda */
