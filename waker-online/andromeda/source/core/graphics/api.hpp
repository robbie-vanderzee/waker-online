#pragma once

#include "core/core.hpp"
#include "core/system/interface/window/window.hpp"

namespace Andromeda {
    namespace Graphics {
        class API {
          public:
            enum class Type {
                None = 0,
                Vulkan = 1
            };
          public:
            virtual ~API() = default;

            virtual void initialize() = 0;
            virtual void process() = 0;
            virtual void shutdown() = 0;

            virtual void set_window_context(std::shared_ptr<Window> window) = 0;

            static Type get_API_Type() {
                return s_API;
            }
            static std::unique_ptr<API> create_API();

          private:
            static Type s_API;
        };
    } /* Graphics */
} /* Andromeda */
