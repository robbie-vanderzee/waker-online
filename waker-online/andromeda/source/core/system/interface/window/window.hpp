#pragma once

#include "core/core.hpp"
#include "core/system/events/event.hpp"

namespace Andromeda {
    struct Window_Properties {
        std::string title;
        unsigned int width, height, x, y;

        Window_Properties(const std::string & title = "Andromeda::Window",
                          unsigned int width = 1920,
                          unsigned int height = 1080,
                          unsigned int x = 0,
                          unsigned int y = 0
                         ) : title(title), width(width), height(height), x(x), y(y) {}
    };
    class Window {
      public:
        using Event_Callback_Function = std::function<void(Event::Event & )>;

        virtual ~Window() = default;

        virtual void on_update() = 0;

        virtual unsigned int get_width() const = 0;
        virtual unsigned int get_height() const = 0;

        // Attributes

        virtual void set_event_callback(const Event_Callback_Function & callback) = 0;
        virtual void set_v_sync(bool vsync_on) = 0;
        virtual bool is_v_synced() const = 0;

        virtual std::any get_native_window() const = 0;

        static std::shared_ptr<Window> create_window(const Window_Properties & properties = Window_Properties());
    };
} /* Andromeda */
