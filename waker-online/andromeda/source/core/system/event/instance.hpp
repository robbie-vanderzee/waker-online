#pragma once

#include "event.hpp"

namespace Andromeda {
    namespace Event {
        namespace Window {
            class Close : public Event {
              public:
                Close() = default;

                EVENT_CLASS_TYPE(Type::Window_Close)
                EVENT_CLASS_CATEGORY(Category::Instance)
            };

            class Resize : public Event {
              public:
                Resize(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

                inline unsigned int get_window_width() const {
                    return m_Width;
                }
                inline unsigned int get_window_height() const {
                    return m_Height;
                }

                std::string to_string() const override {
                    return fmt::format("Window Resize: {0}, {1}", m_Width, m_Height);
                }

                EVENT_CLASS_TYPE(Type::Window_Resize)
                EVENT_CLASS_CATEGORY(Category::Instance)
              private:
                unsigned int m_Width, m_Height;
            };

            class Move : public Event {
              public:
                Move(int x, int y) : m_X(x), m_Y(y) {}

                inline int get_window_x() const {
                    return m_X;
                }
                inline int get_window_y() const {
                    return m_Y;
                }

                std::string to_string() const override {
                    return fmt::format("Window Move: {0}, {1}", m_X, m_Y);
                }

                EVENT_CLASS_TYPE(Type::Window_Move)
                EVENT_CLASS_CATEGORY(Category::Instance)
              private:
                int m_X, m_Y;
            };
        } /* Window */

        namespace Instance {
            class Tick : public Event {
              public:
                Tick() = default;

                EVENT_CLASS_TYPE(Type::Instance_Tick)
                EVENT_CLASS_CATEGORY(Category::Instance)
            };

            class Update : public Event {
              public:
                Update() = default;

                EVENT_CLASS_TYPE(Type::Instance_Update)
                EVENT_CLASS_CATEGORY(Category::Instance)
            };

            class Render : public Event {
              public:
                Render() = default;

                EVENT_CLASS_TYPE(Type::Instance_Render)
                EVENT_CLASS_CATEGORY(Category::Instance)
            };
        } /* Instance */
    } /* Event */
} /* Andromeda */
