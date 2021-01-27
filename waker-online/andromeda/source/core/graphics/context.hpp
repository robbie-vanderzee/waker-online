#pragma once

#include "core/core.hpp"

namespace Andromeda {
    namespace Graphics {
        class Context {
          public:
            virtual ~Context() = default;
            virtual void initialize() = 0;

            static std::unique_ptr<Context> create_context(void * window);
        };
    } /* Graphics */
} /* Andromeda */
