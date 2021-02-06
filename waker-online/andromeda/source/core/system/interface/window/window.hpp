#pragma once

#include "core/core.hpp"
#include "core/system/event/event.hpp"

namespace Andromeda {
    class Window {
      public:
        struct Viewport {
            unsigned int width, height;
        };
        struct Position {
            int x, y;
        };
        struct Properties {
            std::string title;
            Viewport viewport;
            Position position;

            Properties(const std::string & title = "Andromeda::Window",
                       unsigned int width = 1920,
                       unsigned int height = 1080,
                       int x = 0,
                       int y = 0
                      ) : title(title), viewport({width, height}), position({x, y}) {}
        };
      public:
        using Event_Callback_Function = std::function<void (Event::Event &) >;

        virtual ~Window() = default;

        virtual void on_update() = 0;

        virtual Window::Viewport get_viewport() const = 0;
        virtual unsigned int get_width() const = 0;
        virtual unsigned int get_height() const = 0;

        // Attributes

        virtual void set_event_callback(const Event_Callback_Function & callback) = 0;

        virtual std::any get_native_window() const = 0;

        static std::shared_ptr<Window> create_window(const Window::Properties & properties = Window::Properties());
    };
} /* Andromeda */
