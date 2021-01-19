#pragma once

namespace Andromeda {
    typedef enum class Mouse_Code : uint16_t {
        // From glfw3.h
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

    inline std::ostream& operator<<(std::ostream& os, Mouse_Code mouse_code) {
        os << static_cast<int32_t>(mouse_code);
        return os;
    }
} /* Andromeda */


#define ANDROMEDA_MOUSE_BUTTON_0      ::Andromeda::Mouse::Button0
#define ANDROMEDA_MOUSE_BUTTON_1      ::Andromeda::Mouse::Button1
#define ANDROMEDA_MOUSE_BUTTON_2      ::Andromeda::Mouse::Button2
#define ANDROMEDA_MOUSE_BUTTON_3      ::Andromeda::Mouse::Button3
#define ANDROMEDA_MOUSE_BUTTON_4      ::Andromeda::Mouse::Button4
#define ANDROMEDA_MOUSE_BUTTON_5      ::Andromeda::Mouse::Button5
#define ANDROMEDA_MOUSE_BUTTON_6      ::Andromeda::Mouse::Button6
#define ANDROMEDA_MOUSE_BUTTON_7      ::Andromeda::Mouse::Button7
#define ANDROMEDA_MOUSE_BUTTON_LAST   ::Andromeda::Mouse::ButtonLast
#define ANDROMEDA_MOUSE_BUTTON_LEFT   ::Andromeda::Mouse::ButtonLeft
#define ANDROMEDA_MOUSE_BUTTON_RIGHT  ::Andromeda::Mouse::ButtonRight
#define ANDROMEDA_MOUSE_BUTTON_MIDDLE ::Andromeda::Mouse::ButtonMiddle
