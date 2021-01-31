#pragma once

#include "event.hpp"
#include "core/system/input/codes/mouse_code.hpp"

namespace Andromeda {
    namespace Event {
        class Mouse_Move : public Event {
          public:
            Mouse_Move(int x, int y) : m_Mouse_X(x), m_Mouse_Y(y) {}

            inline double get_x() const {
                return m_Mouse_X;
            }

            inline double get_y() const {
                return m_Mouse_Y;
            }

            std::string to_string() const override {
                std::stringstream ss;
                ss << "Mouse Move: " << m_Mouse_X << ", " << m_Mouse_Y;
                return ss.str();
            }

            EVENT_CLASS_TYPE(Mouse_Move)
            EVENT_CLASS_CATEGORY(Event_Category_Input | Event_Category_Mouse)

          private:
            int m_Mouse_X, m_Mouse_Y;
        };

        class Mouse_Scroll : public Event {
          public:
            Mouse_Scroll(double x_Offset, double y_Offset) : m_X_Offset(x_Offset), m_Y_Offset(y_Offset) {}

            inline double get_x_offset() const {
                return m_X_Offset;
            }

            inline double get_y_offset() const {
                return m_Y_Offset;
            }

            std::string to_string() const override {
                std::stringstream ss;
                ss << "Mouse scroll: " << m_X_Offset << ", " << m_Y_Offset;
                return ss.str();
            }

            EVENT_CLASS_TYPE(Mouse_Scroll)
            EVENT_CLASS_CATEGORY(Event_Category_Input | Event_Category_Mouse)
          private:
            double m_X_Offset, m_Y_Offset;
        };

        class Mouse_Button : public Event {
          public:
            inline Mouse_Code get_mouse_button() const {
                return m_Button;
            }

            EVENT_CLASS_CATEGORY(Event_Category_Input | Event_Category_Mouse)
          protected:
            Mouse_Button(Mouse_Code button)
                : m_Button(button) {}
            Mouse_Code m_Button;
        };

        class Mouse_Button_Press : public Mouse_Button {
          public:
            Mouse_Button_Press(Mouse_Code button) : Mouse_Button(button) {}

            std::string to_string() const override {
                std::stringstream ss;
                ss << "Mouse button press: " << m_Button;
                return ss.str();
            }

            EVENT_CLASS_TYPE(Mouse_Button_Press)
        };

        class Mouse_Button_Release : public Mouse_Button {
          public:
            Mouse_Button_Release(Mouse_Code button)
                : Mouse_Button(button) {}

            std::string to_string() const override {
                std::stringstream ss;
                ss << "Mouse button release: " << m_Button;
                return ss.str();
            }

            EVENT_CLASS_TYPE(Mouse_Button_Release)
        };
    } /* Event */
} /* Andromeda */
