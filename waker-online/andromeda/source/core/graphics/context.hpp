#pragma once

#include "core/core.hpp"
#include "core/system/interface/window/window.hpp"

namespace Andromeda {
    namespace Graphics {
        class Context {
          public:
            virtual ~Context() = default;
            virtual void initialize(void * instance) = 0;
            virtual void shutdown() = 0;

            static std::unique_ptr<Context> create_context(std::shared_ptr<Window> window);
        };
    } /* Graphics */
} /* Andromeda */
