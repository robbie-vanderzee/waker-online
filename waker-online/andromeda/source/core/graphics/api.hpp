#pragma once

#include "core/core.hpp"

namespace Andromeda {
    namespace Graphics {
        enum class API_TYPE {
            None = 0, Vulkan = 1
        };
        class API {
          public:
            virtual ~API() = default;

            virtual void initialize() = 0;
            virtual void process() = 0;
            virtual void shutdown() = 0;

            static API_TYPE get_API() {
                return s_API;
            }
            static std::unique_ptr<API> create_API();

          private:
            static API_TYPE s_API;
        };
    } /* Graphics */
} /* Andromeda */
