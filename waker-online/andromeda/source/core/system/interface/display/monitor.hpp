#pragma once

#include "core/core.hpp"

namespace Andromeda {
    class Monitor {
      public:
        struct Position {
            int x, y;
        };
        struct Area {
            int x, y;
            int width, height;
        };
        struct Mode {
            int red, green, blue;
            int refresh_rate;
            int width, height;
        };
      public:
        virtual ~Monitor() = default;
        // Attributes
        virtual std::any get_native_monitor() const = 0;
    };

} /* Andromeda */
