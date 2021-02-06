#pragma once

#include "code/input.hpp"

#include "core/system/interface/window/window.hpp"

namespace Andromeda {
    namespace Input {
        namespace Mouse {
            struct Position {
                double x, y;
            };
        }
        bool is_key_pressed(const std::unique_ptr<Window> & window, Code::Key key);
        bool is_mouse_button_pressed(const std::unique_ptr<Window> & window, Code::Mouse button);
        Mouse::Position get_mouse_position(const std::unique_ptr<Window> & window);
        double get_mouse_x(const std::unique_ptr<Window> & window);
        double get_mouse_y(const std::unique_ptr<Window> & window);
    } /* Input */
} /* Andromeda */
