#pragma once

#include "core/core.hpp"
#include "core/system/events/event.hpp"

namespace Andromeda {
    struct Window_Properties {
        std::string title;
        uint32_t width, height, x, y;

        Window_Properties(const std::string& title = "Andromeda /env",
                          uint32_t width = 1920,
                          uint32_t height = 1080,
                          uint32_t x = 0,
                          uint32_t y = 0
                         ) : title(title), width(width), height(height), x(x), y(y) {}
    };
    class Window {
      public:
        using Event_Callback_Function = std::function<void(Event::Event & )>;

        virtual ~Window() {}

        virtual void on_update() = 0;

        virtual uint32_t get_width() const = 0;
        virtual uint32_t get_height() const = 0;

        // Attributes

        virtual void set_event_callback(const Event_Callback_Function & callback) = 0;
        virtual void set_v_sync(bool vsync_on) = 0;
        virtual bool is_v_synced() const = 0;

        virtual void* get_native_window() const = 0;

        static std::unique_ptr<Window> create(const Window_Properties & properties = Window_Properties());
    };
} /* Andromeda */
