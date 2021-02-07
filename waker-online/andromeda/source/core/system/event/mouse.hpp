#pragma once

#include "event.hpp"

#include "core/system/input/code/mouse.hpp"

namespace Andromeda {
    namespace Event {
        namespace Mouse {
            class Move : public Event {
              public:
                Move(int x, int y) : m_Mouse_X(x), m_Mouse_Y(y) {}

                inline double get_x() const {
                    return m_Mouse_X;
                }

                inline double get_y() const {
                    return m_Mouse_Y;
                }

                std::string to_string() const override {
                    return fmt::format("Mouse Move: {0}, {1}", m_Mouse_X, m_Mouse_Y);
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Move)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Input | Andromeda::Event::Category::Mouse)
              private:
                int m_Mouse_X, m_Mouse_Y;
            };

            class Scroll : public Event {
              public:
                Scroll(double x_Offset, double y_Offset) : m_X_Offset(x_Offset), m_Y_Offset(y_Offset) {}

                inline double get_x_offset() const {
                    return m_X_Offset;
                }

                inline double get_y_offset() const {
                    return m_Y_Offset;
                }

                std::string to_string() const override {
                    return fmt::format("Mouse Scroll: {0}, {1}", m_X_Offset, m_Y_Offset);
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Scroll)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Input | Andromeda::Event::Category::Mouse)
              private:
                double m_X_Offset, m_Y_Offset;
            };

            class Button : public Event {
              public:
                class Press;
                class Release;
              public:
                inline Input::Code::Mouse get_mouse_button() const {
                    return m_Button;
                }

                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Input | Andromeda::Event::Category::Mouse | Andromeda::Event::Category::Button)
              protected:
                Button(Input::Code::Mouse button)
                    : m_Button(button) {}
                Input::Code::Mouse m_Button;
            };

            class Button::Press : public Button {
              public:
                Press(Input::Code::Mouse button) : Button(button) {}

                std::string to_string() const override {
                    return fmt::format("Mouse Button Press: {0}", m_Button);
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Press)
            };

            class Button::Release : public Button {
              public:
                Release(Input::Code::Mouse button)
                    : Button(button) {}

                std::string to_string() const override {
                    return fmt::format("Mouse Button Release: {0}", m_Button);
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Release)
            };

        } /* Mouse */
    } /* Event */
} /* Andromeda */
