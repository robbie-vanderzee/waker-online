#pragma once

#include "core/core.hpp"

#include "code/input.hpp"

namespace Andromeda {
    namespace Input {
        namespace Mouse {
            struct Position {
                double x, y;
            };
        }
        class Manager {
          public:
            virtual ~Manager() = default;

            virtual bool is_key_pressed(Code::Key key) = 0;
            virtual bool is_mouse_button_pressed(Code::Mouse button) = 0;
            virtual Mouse::Position get_mouse_position() = 0;
        };
    } /* Input */
} /* Andromeda */
