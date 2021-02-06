#pragma once

namespace Andromeda {
    namespace Input {
        namespace Code {
            typedef enum class Key : unsigned int {
                Space               = 32,
                Apostrophe          = 39, /* ' */
                Comma               = 44, /* , */
                Minus               = 45, /* - */
                Period              = 46, /* . */
                Slash               = 47, /* / */

                D0                  = 48, /* 0 */
                D1                  = 49, /* 1 */
                D2                  = 50, /* 2 */
                D3                  = 51, /* 3 */
                D4                  = 52, /* 4 */
                D5                  = 53, /* 5 */
                D6                  = 54, /* 6 */
                D7                  = 55, /* 7 */
                D8                  = 56, /* 8 */
                D9                  = 57, /* 9 */

                Semicolon           = 59, /* ; */
                Equal               = 61, /* = */

                A                   = 65,
                B                   = 66,
                C                   = 67,
                D                   = 68,
                E                   = 69,
                F                   = 70,
                G                   = 71,
                H                   = 72,
                I                   = 73,
                J                   = 74,
                K                   = 75,
                L                   = 76,
                M                   = 77,
                N                   = 78,
                O                   = 79,
                P                   = 80,
                Q                   = 81,
                R                   = 82,
                S                   = 83,
                T                   = 84,
                U                   = 85,
                V                   = 86,
                W                   = 87,
                X                   = 88,
                Y                   = 89,
                Z                   = 90,

                LeftBracket         = 91,  /* [ */
                Backslash           = 92,  /* \ */
                RightBracket        = 93,  /* ] */
                GraveAccent         = 96,  /* ` */

                World1              = 161, /* non-US #1 */
                World2              = 162, /* non-US #2 */

                /* Function keys */
                Escape              = 256,
                Enter               = 257,
                Tab                 = 258,
                Backspace           = 259,
                Insert              = 260,
                Delete              = 261,
                Right               = 262,
                Left                = 263,
                Down                = 264,
                Up                  = 265,
                PageUp              = 266,
                PageDown            = 267,
                Home                = 268,
                End                 = 269,
                CapsLock            = 280,
                ScrollLock          = 281,
                NumLock             = 282,
                PrintScreen         = 283,
                Pause               = 284,
                F1                  = 290,
                F2                  = 291,
                F3                  = 292,
                F4                  = 293,
                F5                  = 294,
                F6                  = 295,
                F7                  = 296,
                F8                  = 297,
                F9                  = 298,
                F10                 = 299,
                F11                 = 300,
                F12                 = 301,
                F13                 = 302,
                F14                 = 303,
                F15                 = 304,
                F16                 = 305,
                F17                 = 306,
                F18                 = 307,
                F19                 = 308,
                F20                 = 309,
                F21                 = 310,
                F22                 = 311,
                F23                 = 312,
                F24                 = 313,
                F25                 = 314,

                /* Keypad */
                KP0                 = 320,
                KP1                 = 321,
                KP2                 = 322,
                KP3                 = 323,
                KP4                 = 324,
                KP5                 = 325,
                KP6                 = 326,
                KP7                 = 327,
                KP8                 = 328,
                KP9                 = 329,
                KPDecimal           = 330,
                KPDivide            = 331,
                KPMultiply          = 332,
                KPSubtract          = 333,
                KPAdd               = 334,
                KPEnter             = 335,
                KPEqual             = 336,

                LeftShift           = 340,
                LeftControl         = 341,
                LeftAlt             = 342,
                LeftSuper           = 343,
                RightShift          = 344,
                RightControl        = 345,
                RightAlt            = 346,
                RightSuper          = 347,
                Menu                = 348
            } Key;

            inline std::ostream & operator<< (std::ostream & os, Key code) {
                os << static_cast<int>(code);
                return os;
            }
        } /* Code */
    } /* Input */
} /* Andromeda */

// From glfw3.h
#define ANDROMEDA_KEY_SPACE           ::Andromeda::Input::Code::Key::Space
#define ANDROMEDA_KEY_APOSTROPHE      ::Andromeda::Input::Code::Key::Apostrophe    /* ' */
#define ANDROMEDA_KEY_COMMA           ::Andromeda::Input::Code::Key::Comma         /* , */
#define ANDROMEDA_KEY_MINUS           ::Andromeda::Input::Code::Key::Minus         /* - */
#define ANDROMEDA_KEY_PERIOD          ::Andromeda::Input::Code::Key::Period        /* . */
#define ANDROMEDA_KEY_SLASH           ::Andromeda::Input::Code::Key::Slash         /* / */
#define ANDROMEDA_KEY_0               ::Andromeda::Input::Code::Key::D0
#define ANDROMEDA_KEY_1               ::Andromeda::Input::Code::Key::D1
#define ANDROMEDA_KEY_2               ::Andromeda::Input::Code::Key::D2
#define ANDROMEDA_KEY_3               ::Andromeda::Input::Code::Key::D3
#define ANDROMEDA_KEY_4               ::Andromeda::Input::Code::Key::D4
#define ANDROMEDA_KEY_5               ::Andromeda::Input::Code::Key::D5
#define ANDROMEDA_KEY_6               ::Andromeda::Input::Code::Key::D6
#define ANDROMEDA_KEY_7               ::Andromeda::Input::Code::Key::D7
#define ANDROMEDA_KEY_8               ::Andromeda::Input::Code::Key::D8
#define ANDROMEDA_KEY_9               ::Andromeda::Input::Code::Key::D9
#define ANDROMEDA_KEY_SEMICOLON       ::Andromeda::Input::Code::Key::Semicolon     /* ; */
#define ANDROMEDA_KEY_EQUAL           ::Andromeda::Input::Code::Key::Equal         /* = */
#define ANDROMEDA_KEY_A               ::Andromeda::Input::Code::Key::A
#define ANDROMEDA_KEY_B               ::Andromeda::Input::Code::Key::B
#define ANDROMEDA_KEY_C               ::Andromeda::Input::Code::Key::C
#define ANDROMEDA_KEY_D               ::Andromeda::Input::Code::Key::D
#define ANDROMEDA_KEY_E               ::Andromeda::Input::Code::Key::E
#define ANDROMEDA_KEY_F               ::Andromeda::Input::Code::Key::F
#define ANDROMEDA_KEY_G               ::Andromeda::Input::Code::Key::G
#define ANDROMEDA_KEY_H               ::Andromeda::Input::Code::Key::H
#define ANDROMEDA_KEY_I               ::Andromeda::Input::Code::Key::I
#define ANDROMEDA_KEY_J               ::Andromeda::Input::Code::Key::J
#define ANDROMEDA_KEY_K               ::Andromeda::Input::Code::Key::K
#define ANDROMEDA_KEY_L               ::Andromeda::Input::Code::Key::L
#define ANDROMEDA_KEY_M               ::Andromeda::Input::Code::Key::M
#define ANDROMEDA_KEY_N               ::Andromeda::Input::Code::Key::N
#define ANDROMEDA_KEY_O               ::Andromeda::Input::Code::Key::O
#define ANDROMEDA_KEY_P               ::Andromeda::Input::Code::Key::P
#define ANDROMEDA_KEY_Q               ::Andromeda::Input::Code::Key::Q
#define ANDROMEDA_KEY_R               ::Andromeda::Input::Code::Key::R
#define ANDROMEDA_KEY_S               ::Andromeda::Input::Code::Key::S
#define ANDROMEDA_KEY_T               ::Andromeda::Input::Code::Key::T
#define ANDROMEDA_KEY_U               ::Andromeda::Input::Code::Key::U
#define ANDROMEDA_KEY_V               ::Andromeda::Input::Code::Key::V
#define ANDROMEDA_KEY_W               ::Andromeda::Input::Code::Key::W
#define ANDROMEDA_KEY_X               ::Andromeda::Input::Code::Key::X
#define ANDROMEDA_KEY_Y               ::Andromeda::Input::Code::Key::Y
#define ANDROMEDA_KEY_Z               ::Andromeda::Input::Code::Key::Z
#define ANDROMEDA_KEY_LEFT_BRACKET    ::Andromeda::Input::Code::Key::LeftBracket   /* [ */
#define ANDROMEDA_KEY_BACKSLASH       ::Andromeda::Input::Code::Key::Backslash     /* \ */
#define ANDROMEDA_KEY_RIGHT_BRACKET   ::Andromeda::Input::Code::Key::RightBracket  /* ] */
#define ANDROMEDA_KEY_GRAVE_ACCENT    ::Andromeda::Input::Code::Key::GraveAccent   /* ` */
#define ANDROMEDA_KEY_WORLD_1         ::Andromeda::Input::Code::Key::World1        /* non-US #1 */
#define ANDROMEDA_KEY_WORLD_2         ::Andromeda::Input::Code::Key::World2        /* non-US #2 */

/* Function keys */
#define ANDROMEDA_KEY_ESCAPE          ::Andromeda::Input::Code::Key::Escape
#define ANDROMEDA_KEY_ENTER           ::Andromeda::Input::Code::Key::Enter
#define ANDROMEDA_KEY_TAB             ::Andromeda::Input::Code::Key::Tab
#define ANDROMEDA_KEY_BACKSPACE       ::Andromeda::Input::Code::Key::Backspace
#define ANDROMEDA_KEY_INSERT          ::Andromeda::Input::Code::Key::Insert
#define ANDROMEDA_KEY_DELETE          ::Andromeda::Input::Code::Key::Delete
#define ANDROMEDA_KEY_RIGHT           ::Andromeda::Input::Code::Key::Right
#define ANDROMEDA_KEY_LEFT            ::Andromeda::Input::Code::Key::Left
#define ANDROMEDA_KEY_DOWN            ::Andromeda::Input::Code::Key::Down
#define ANDROMEDA_KEY_UP              ::Andromeda::Input::Code::Key::Up
#define ANDROMEDA_KEY_PAGE_UP         ::Andromeda::Input::Code::Key::PageUp
#define ANDROMEDA_KEY_PAGE_DOWN       ::Andromeda::Input::Code::Key::PageDown
#define ANDROMEDA_KEY_HOME            ::Andromeda::Input::Code::Key::Home
#define ANDROMEDA_KEY_END             ::Andromeda::Input::Code::Key::End
#define ANDROMEDA_KEY_CAPS_LOCK       ::Andromeda::Input::Code::Key::CapsLock
#define ANDROMEDA_KEY_SCROLL_LOCK     ::Andromeda::Input::Code::Key::ScrollLock
#define ANDROMEDA_KEY_NUM_LOCK        ::Andromeda::Input::Code::Key::NumLock
#define ANDROMEDA_KEY_PRINT_SCREEN    ::Andromeda::Input::Code::Key::PrintScreen
#define ANDROMEDA_KEY_PAUSE           ::Andromeda::Input::Code::Key::Pause
#define ANDROMEDA_KEY_F1              ::Andromeda::Input::Code::Key::F1
#define ANDROMEDA_KEY_F2              ::Andromeda::Input::Code::Key::F2
#define ANDROMEDA_KEY_F3              ::Andromeda::Input::Code::Key::F3
#define ANDROMEDA_KEY_F4              ::Andromeda::Input::Code::Key::F4
#define ANDROMEDA_KEY_F5              ::Andromeda::Input::Code::Key::F5
#define ANDROMEDA_KEY_F6              ::Andromeda::Input::Code::Key::F6
#define ANDROMEDA_KEY_F7              ::Andromeda::Input::Code::Key::F7
#define ANDROMEDA_KEY_F8              ::Andromeda::Input::Code::Key::F8
#define ANDROMEDA_KEY_F9              ::Andromeda::Input::Code::Key::F9
#define ANDROMEDA_KEY_F10             ::Andromeda::Input::Code::Key::F10
#define ANDROMEDA_KEY_F11             ::Andromeda::Input::Code::Key::F11
#define ANDROMEDA_KEY_F12             ::Andromeda::Input::Code::Key::F12
#define ANDROMEDA_KEY_F13             ::Andromeda::Input::Code::Key::F13
#define ANDROMEDA_KEY_F14             ::Andromeda::Input::Code::Key::F14
#define ANDROMEDA_KEY_F15             ::Andromeda::Input::Code::Key::F15
#define ANDROMEDA_KEY_F16             ::Andromeda::Input::Code::Key::F16
#define ANDROMEDA_KEY_F17             ::Andromeda::Input::Code::Key::F17
#define ANDROMEDA_KEY_F18             ::Andromeda::Input::Code::Key::F18
#define ANDROMEDA_KEY_F19             ::Andromeda::Input::Code::Key::F19
#define ANDROMEDA_KEY_F20             ::Andromeda::Input::Code::Key::F20
#define ANDROMEDA_KEY_F21             ::Andromeda::Input::Code::Key::F21
#define ANDROMEDA_KEY_F22             ::Andromeda::Input::Code::Key::F22
#define ANDROMEDA_KEY_F23             ::Andromeda::Input::Code::Key::F23
#define ANDROMEDA_KEY_F24             ::Andromeda::Input::Code::Key::F24
#define ANDROMEDA_KEY_F25             ::Andromeda::Input::Code::Key::F25

/* Keypad */
#define ANDROMEDA_KEY_KP_0            ::Andromeda::Input::Code::Key::KP0
#define ANDROMEDA_KEY_KP_1            ::Andromeda::Input::Code::Key::KP1
#define ANDROMEDA_KEY_KP_2            ::Andromeda::Input::Code::Key::KP2
#define ANDROMEDA_KEY_KP_3            ::Andromeda::Input::Code::Key::KP3
#define ANDROMEDA_KEY_KP_4            ::Andromeda::Input::Code::Key::KP4
#define ANDROMEDA_KEY_KP_5            ::Andromeda::Input::Code::Key::KP5
#define ANDROMEDA_KEY_KP_6            ::Andromeda::Input::Code::Key::KP6
#define ANDROMEDA_KEY_KP_7            ::Andromeda::Input::Code::Key::KP7
#define ANDROMEDA_KEY_KP_8            ::Andromeda::Input::Code::Key::KP8
#define ANDROMEDA_KEY_KP_9            ::Andromeda::Input::Code::Key::KP9
#define ANDROMEDA_KEY_KP_DECIMAL      ::Andromeda::Input::Code::Key::KPDecimal
#define ANDROMEDA_KEY_KP_DIVIDE       ::Andromeda::Input::Code::Key::KPDivide
#define ANDROMEDA_KEY_KP_MULTIPLY     ::Andromeda::Input::Code::Key::KPMultiply
#define ANDROMEDA_KEY_KP_SUBTRACT     ::Andromeda::Input::Code::Key::KPSubtract
#define ANDROMEDA_KEY_KP_ADD          ::Andromeda::Input::Code::Key::KPAdd
#define ANDROMEDA_KEY_KP_ENTER        ::Andromeda::Input::Code::Key::KPEnter
#define ANDROMEDA_KEY_KP_EQUAL        ::Andromeda::Input::Code::Key::KPEqual

#define ANDROMEDA_KEY_LEFT_SHIFT      ::Andromeda::Input::Code::Key::LeftShift
#define ANDROMEDA_KEY_LEFT_CONTROL    ::Andromeda::Input::Code::Key::LeftControl
#define ANDROMEDA_KEY_LEFT_ALT        ::Andromeda::Input::Code::Key::LeftAlt
#define ANDROMEDA_KEY_LEFT_SUPER      ::Andromeda::Input::Code::Key::LeftSuper
#define ANDROMEDA_KEY_RIGHT_SHIFT     ::Andromeda::Input::Code::Key::RightShift
#define ANDROMEDA_KEY_RIGHT_CONTROL   ::Andromeda::Input::Code::Key::RightControl
#define ANDROMEDA_KEY_RIGHT_ALT       ::Andromeda::Input::Code::Key::RightAlt
#define ANDROMEDA_KEY_RIGHT_SUPER     ::Andromeda::Input::Code::Key::RightSuper
#define ANDROMEDA_KEY_MENU            ::Andromeda::Input::Code::Key::Menu
