#pragma once

namespace Andromeda {
    typedef enum class Key_Code : unsigned int {
        // From glfw3.h
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

    inline std::ostream & operator<< (std::ostream & os, Key_Code key_code) {
        os << static_cast<int>(key_code);
        return os;
    }
} /* Andromeda */

// From glfw3.h
#define ANDROMEDA_KEY_SPACE           ::Andromeda::Key::Space
#define ANDROMEDA_KEY_APOSTROPHE      ::Andromeda::Key::Apostrophe    /* ' */
#define ANDROMEDA_KEY_COMMA           ::Andromeda::Key::Comma         /* , */
#define ANDROMEDA_KEY_MINUS           ::Andromeda::Key::Minus         /* - */
#define ANDROMEDA_KEY_PERIOD          ::Andromeda::Key::Period        /* . */
#define ANDROMEDA_KEY_SLASH           ::Andromeda::Key::Slash         /* / */
#define ANDROMEDA_KEY_0               ::Andromeda::Key::D0
#define ANDROMEDA_KEY_1               ::Andromeda::Key::D1
#define ANDROMEDA_KEY_2               ::Andromeda::Key::D2
#define ANDROMEDA_KEY_3               ::Andromeda::Key::D3
#define ANDROMEDA_KEY_4               ::Andromeda::Key::D4
#define ANDROMEDA_KEY_5               ::Andromeda::Key::D5
#define ANDROMEDA_KEY_6               ::Andromeda::Key::D6
#define ANDROMEDA_KEY_7               ::Andromeda::Key::D7
#define ANDROMEDA_KEY_8               ::Andromeda::Key::D8
#define ANDROMEDA_KEY_9               ::Andromeda::Key::D9
#define ANDROMEDA_KEY_SEMICOLON       ::Andromeda::Key::Semicolon     /* ; */
#define ANDROMEDA_KEY_EQUAL           ::Andromeda::Key::Equal         /* = */
#define ANDROMEDA_KEY_A               ::Andromeda::Key::A
#define ANDROMEDA_KEY_B               ::Andromeda::Key::B
#define ANDROMEDA_KEY_C               ::Andromeda::Key::C
#define ANDROMEDA_KEY_D               ::Andromeda::Key::D
#define ANDROMEDA_KEY_E               ::Andromeda::Key::E
#define ANDROMEDA_KEY_F               ::Andromeda::Key::F
#define ANDROMEDA_KEY_G               ::Andromeda::Key::G
#define ANDROMEDA_KEY_H               ::Andromeda::Key::H
#define ANDROMEDA_KEY_I               ::Andromeda::Key::I
#define ANDROMEDA_KEY_J               ::Andromeda::Key::J
#define ANDROMEDA_KEY_K               ::Andromeda::Key::K
#define ANDROMEDA_KEY_L               ::Andromeda::Key::L
#define ANDROMEDA_KEY_M               ::Andromeda::Key::M
#define ANDROMEDA_KEY_N               ::Andromeda::Key::N
#define ANDROMEDA_KEY_O               ::Andromeda::Key::O
#define ANDROMEDA_KEY_P               ::Andromeda::Key::P
#define ANDROMEDA_KEY_Q               ::Andromeda::Key::Q
#define ANDROMEDA_KEY_R               ::Andromeda::Key::R
#define ANDROMEDA_KEY_S               ::Andromeda::Key::S
#define ANDROMEDA_KEY_T               ::Andromeda::Key::T
#define ANDROMEDA_KEY_U               ::Andromeda::Key::U
#define ANDROMEDA_KEY_V               ::Andromeda::Key::V
#define ANDROMEDA_KEY_W               ::Andromeda::Key::W
#define ANDROMEDA_KEY_X               ::Andromeda::Key::X
#define ANDROMEDA_KEY_Y               ::Andromeda::Key::Y
#define ANDROMEDA_KEY_Z               ::Andromeda::Key::Z
#define ANDROMEDA_KEY_LEFT_BRACKET    ::Andromeda::Key::LeftBracket   /* [ */
#define ANDROMEDA_KEY_BACKSLASH       ::Andromeda::Key::Backslash     /* \ */
#define ANDROMEDA_KEY_RIGHT_BRACKET   ::Andromeda::Key::RightBracket  /* ] */
#define ANDROMEDA_KEY_GRAVE_ACCENT    ::Andromeda::Key::GraveAccent   /* ` */
#define ANDROMEDA_KEY_WORLD_1         ::Andromeda::Key::World1        /* non-US #1 */
#define ANDROMEDA_KEY_WORLD_2         ::Andromeda::Key::World2        /* non-US #2 */

/* Function keys */
#define ANDROMEDA_KEY_ESCAPE          ::Andromeda::Key::Escape
#define ANDROMEDA_KEY_ENTER           ::Andromeda::Key::Enter
#define ANDROMEDA_KEY_TAB             ::Andromeda::Key::Tab
#define ANDROMEDA_KEY_BACKSPACE       ::Andromeda::Key::Backspace
#define ANDROMEDA_KEY_INSERT          ::Andromeda::Key::Insert
#define ANDROMEDA_KEY_DELETE          ::Andromeda::Key::Delete
#define ANDROMEDA_KEY_RIGHT           ::Andromeda::Key::Right
#define ANDROMEDA_KEY_LEFT            ::Andromeda::Key::Left
#define ANDROMEDA_KEY_DOWN            ::Andromeda::Key::Down
#define ANDROMEDA_KEY_UP              ::Andromeda::Key::Up
#define ANDROMEDA_KEY_PAGE_UP         ::Andromeda::Key::PageUp
#define ANDROMEDA_KEY_PAGE_DOWN       ::Andromeda::Key::PageDown
#define ANDROMEDA_KEY_HOME            ::Andromeda::Key::Home
#define ANDROMEDA_KEY_END             ::Andromeda::Key::End
#define ANDROMEDA_KEY_CAPS_LOCK       ::Andromeda::Key::CapsLock
#define ANDROMEDA_KEY_SCROLL_LOCK     ::Andromeda::Key::ScrollLock
#define ANDROMEDA_KEY_NUM_LOCK        ::Andromeda::Key::NumLock
#define ANDROMEDA_KEY_PRINT_SCREEN    ::Andromeda::Key::PrintScreen
#define ANDROMEDA_KEY_PAUSE           ::Andromeda::Key::Pause
#define ANDROMEDA_KEY_F1              ::Andromeda::Key::F1
#define ANDROMEDA_KEY_F2              ::Andromeda::Key::F2
#define ANDROMEDA_KEY_F3              ::Andromeda::Key::F3
#define ANDROMEDA_KEY_F4              ::Andromeda::Key::F4
#define ANDROMEDA_KEY_F5              ::Andromeda::Key::F5
#define ANDROMEDA_KEY_F6              ::Andromeda::Key::F6
#define ANDROMEDA_KEY_F7              ::Andromeda::Key::F7
#define ANDROMEDA_KEY_F8              ::Andromeda::Key::F8
#define ANDROMEDA_KEY_F9              ::Andromeda::Key::F9
#define ANDROMEDA_KEY_F10             ::Andromeda::Key::F10
#define ANDROMEDA_KEY_F11             ::Andromeda::Key::F11
#define ANDROMEDA_KEY_F12             ::Andromeda::Key::F12
#define ANDROMEDA_KEY_F13             ::Andromeda::Key::F13
#define ANDROMEDA_KEY_F14             ::Andromeda::Key::F14
#define ANDROMEDA_KEY_F15             ::Andromeda::Key::F15
#define ANDROMEDA_KEY_F16             ::Andromeda::Key::F16
#define ANDROMEDA_KEY_F17             ::Andromeda::Key::F17
#define ANDROMEDA_KEY_F18             ::Andromeda::Key::F18
#define ANDROMEDA_KEY_F19             ::Andromeda::Key::F19
#define ANDROMEDA_KEY_F20             ::Andromeda::Key::F20
#define ANDROMEDA_KEY_F21             ::Andromeda::Key::F21
#define ANDROMEDA_KEY_F22             ::Andromeda::Key::F22
#define ANDROMEDA_KEY_F23             ::Andromeda::Key::F23
#define ANDROMEDA_KEY_F24             ::Andromeda::Key::F24
#define ANDROMEDA_KEY_F25             ::Andromeda::Key::F25

/* Keypad */
#define ANDROMEDA_KEY_KP_0            ::Andromeda::Key::KP0
#define ANDROMEDA_KEY_KP_1            ::Andromeda::Key::KP1
#define ANDROMEDA_KEY_KP_2            ::Andromeda::Key::KP2
#define ANDROMEDA_KEY_KP_3            ::Andromeda::Key::KP3
#define ANDROMEDA_KEY_KP_4            ::Andromeda::Key::KP4
#define ANDROMEDA_KEY_KP_5            ::Andromeda::Key::KP5
#define ANDROMEDA_KEY_KP_6            ::Andromeda::Key::KP6
#define ANDROMEDA_KEY_KP_7            ::Andromeda::Key::KP7
#define ANDROMEDA_KEY_KP_8            ::Andromeda::Key::KP8
#define ANDROMEDA_KEY_KP_9            ::Andromeda::Key::KP9
#define ANDROMEDA_KEY_KP_DECIMAL      ::Andromeda::Key::KPDecimal
#define ANDROMEDA_KEY_KP_DIVIDE       ::Andromeda::Key::KPDivide
#define ANDROMEDA_KEY_KP_MULTIPLY     ::Andromeda::Key::KPMultiply
#define ANDROMEDA_KEY_KP_SUBTRACT     ::Andromeda::Key::KPSubtract
#define ANDROMEDA_KEY_KP_ADD          ::Andromeda::Key::KPAdd
#define ANDROMEDA_KEY_KP_ENTER        ::Andromeda::Key::KPEnter
#define ANDROMEDA_KEY_KP_EQUAL        ::Andromeda::Key::KPEqual

#define ANDROMEDA_KEY_LEFT_SHIFT      ::Andromeda::Key::LeftShift
#define ANDROMEDA_KEY_LEFT_CONTROL    ::Andromeda::Key::LeftControl
#define ANDROMEDA_KEY_LEFT_ALT        ::Andromeda::Key::LeftAlt
#define ANDROMEDA_KEY_LEFT_SUPER      ::Andromeda::Key::LeftSuper
#define ANDROMEDA_KEY_RIGHT_SHIFT     ::Andromeda::Key::RightShift
#define ANDROMEDA_KEY_RIGHT_CONTROL   ::Andromeda::Key::RightControl
#define ANDROMEDA_KEY_RIGHT_ALT       ::Andromeda::Key::RightAlt
#define ANDROMEDA_KEY_RIGHT_SUPER     ::Andromeda::Key::RightSuper
#define ANDROMEDA_KEY_MENU            ::Andromeda::Key::Menu
