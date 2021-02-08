#pragma once

#include "core/core.hpp"

#include "core/system/interface/event/event.hpp"

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

        virtual void update() = 0;

        virtual Monitor::Position get_position() const = 0;
        virtual Monitor::Area get_area() const = 0;
        virtual Monitor::Mode get_video_mode() const = 0;
        virtual bool is_primary() const = 0;
        // Attributes
        virtual void set_event_callback(const Event::Callback & callback) = 0;
        virtual std::any get_native_monitor() const = 0;
    };

} /* Andromeda */
