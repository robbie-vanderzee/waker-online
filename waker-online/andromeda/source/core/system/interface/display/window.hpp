#pragma once

#include "core/core.hpp"

#include "core/system/interface/event/event.hpp"
#include "core/system/interface/input/input.hpp"

namespace Andromeda {
    class Window : Input::Manager {
      public:
        struct Viewport {
            unsigned int width, height;
        };
        struct Position {
            int x, y;
        };
        enum class Option : unsigned int {
            None       = 0,
            Decorated  = BIT(0),
            Resizable  = BIT(1),
            Visible    = BIT(2),
            Floating   = BIT(3),
            Fullscreen = BIT(4)
        };

        friend inline constexpr Window::Option operator | (Window::Option lhs, Window::Option rhs) {
            using T = std::underlying_type_t <Window::Option>;
            return static_cast<Window::Option>(static_cast<T>(lhs) | static_cast<T>(rhs));
        }

        friend inline constexpr Window::Option & operator |= (Window::Option & lhs, Window::Option rhs) {
            lhs = lhs | rhs;
            return lhs;
        }

        friend inline constexpr Window::Option operator & (Window::Option lhs, Window::Option rhs) {
            using T = std::underlying_type_t <Window::Option>;
            return static_cast<Window::Option>(static_cast<T>(lhs) & static_cast<T>(rhs));
        }

        friend inline constexpr Window::Option & operator &= (Window::Option & lhs, Window::Option rhs) {
            lhs = lhs & rhs;
            return lhs;
        }

        friend inline constexpr bool operator == (Window::Option lhs, Window::Option rhs) {
            using T = std::underlying_type_t <Window::Option>;
            return static_cast<T>(lhs) & static_cast<T>(rhs);
        }

        struct Properties {
            std::string title;
            Viewport viewport;
            Position position;
            Option options;

            Properties(const std::string & title,
                       Viewport viewport,
                       Position position,
                       Option options
                      ) : title(title), viewport(viewport), position(position), options(options) {
            }
        };
      public:
        virtual ~Window() = default;

        virtual void on_update() = 0;

        virtual Window::Viewport get_viewport() const = 0;
        virtual Window::Position get_position() const = 0;

        // Input State
        virtual bool is_key_pressed(Andromeda::Input::Code::Key key) = 0;
        virtual bool is_mouse_button_pressed(Andromeda::Input::Code::Mouse button) = 0;
        virtual Andromeda::Input::Mouse::Position get_mouse_position() = 0;

        // Attributes
        virtual void set_fullscreen() = 0;
        virtual void set_attributes(Window::Option options) = 0;
        virtual void set_event_callback(const Event::Callback & callback) = 0;
        virtual std::any get_native_window() const = 0;

        static std::shared_ptr<Window> create_window(const Window::Properties & properties);
    };

} /* Andromeda */
