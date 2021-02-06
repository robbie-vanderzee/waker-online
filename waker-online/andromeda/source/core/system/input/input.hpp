#pragma once

#include "code/input.hpp"

namespace Andromeda {
    namespace Input {
        namespace Mouse {
            struct Position {
                double x, y;
            };
        }
        bool is_key_pressed(Code::Key key);
        bool is_mouse_button_pressed(Code::Mouse button);
        Mouse::Position get_mouse_position();
        double get_mouse_x();
        double get_mouse_y();
    } /* Input */
} /* Andromeda */
