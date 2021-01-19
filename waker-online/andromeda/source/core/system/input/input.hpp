#pragma once

#include "codes/input_codes.hpp"

namespace Andromeda {

    class Input {
      public:
        static bool is_key_pressed(Key_Code key);
        static bool is_mouse_button_pressed(Mouse_Code mouse_button);
        static std::pair<float, float> get_mouse_position();
        static float get_mouse_x();
        static float get_mouse_y();
    };

} /* Andromeda */
