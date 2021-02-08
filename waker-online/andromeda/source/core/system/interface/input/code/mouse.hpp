#pragma once

#include "core/core.hpp"

namespace Andromeda {
    namespace Input {
        namespace Code {
            typedef enum class Mouse : unsigned int {
                Button0                = 0,
                Button1                = 1,
                Button2                = 2,
                Button3                = 3,
                Button4                = 4,
                Button5                = 5,
                Button6                = 6,
                Button7                = 7,

                ButtonLast             = Button7,
                ButtonLeft             = Button0,
                ButtonRight            = Button1,
                ButtonMiddle           = Button2
            } Mouse;

            inline std::ostream & operator<< (std::ostream & os, Mouse code) {
                os << static_cast<int>(code);
                return os;
            }
        } /* Code */
    } /* Input */
} /* Andromeda */


#define ANDROMEDA_MOUSE_BUTTON_0      ::Andromeda::Input::Code::Mouse::Button0
#define ANDROMEDA_MOUSE_BUTTON_1      ::Andromeda::Input::Code::Mouse::Button1
#define ANDROMEDA_MOUSE_BUTTON_2      ::Andromeda::Input::Code::Mouse::Button2
#define ANDROMEDA_MOUSE_BUTTON_3      ::Andromeda::Input::Code::Mouse::Button3
#define ANDROMEDA_MOUSE_BUTTON_4      ::Andromeda::Input::Code::Mouse::Button4
#define ANDROMEDA_MOUSE_BUTTON_5      ::Andromeda::Input::Code::Mouse::Button5
#define ANDROMEDA_MOUSE_BUTTON_6      ::Andromeda::Input::Code::Mouse::Button6
#define ANDROMEDA_MOUSE_BUTTON_7      ::Andromeda::Input::Code::Mouse::Button7
#define ANDROMEDA_MOUSE_BUTTON_LAST   ::Andromeda::Input::Code::Mouse::ButtonLast
#define ANDROMEDA_MOUSE_BUTTON_LEFT   ::Andromeda::Input::Code::Mouse::ButtonLeft
#define ANDROMEDA_MOUSE_BUTTON_RIGHT  ::Andromeda::Input::Code::Mouse::ButtonRight
#define ANDROMEDA_MOUSE_BUTTON_MIDDLE ::Andromeda::Input::Code::Mouse::ButtonMiddle
