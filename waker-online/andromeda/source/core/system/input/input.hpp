#pragma once

#include "codes/input_codes.hpp"

namespace Andromeda {
    struct Mouse_Position {
        double x, y;
    };
    class Input {
      public:
        static bool is_key_pressed(Key_Code key);
        static bool is_mouse_button_pressed(Mouse_Code mouse_button);
        static Mouse_Position get_mouse_position();
        static double get_mouse_x();
        static double get_mouse_y();
    };
} /* Andromeda */
