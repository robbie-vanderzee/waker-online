#pragma once

#include "event.hpp"

namespace Andromeda {
    namespace Event {
        class Window_Close : public Event {
          public:
            Window_Close() = default;

            EVENT_CLASS_TYPE(Window_Close)
            EVENT_CLASS_CATEGORY(Event_Category_Instance)
        };

        class Window_Resize : public Event {
          public:
            Window_Resize(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

            inline unsigned int get_window_width() const {
                return m_Width;
            }
            inline unsigned int get_window_height() const {
                return m_Height;
            }

            std::string to_string() const override {
                std::stringstream ss;
                ss << "Window Resize: " << m_Width << ", " << m_Height;
                return ss.str();
            }

            EVENT_CLASS_TYPE(Window_Resize)
            EVENT_CLASS_CATEGORY(Event_Category_Instance)
          private:
            unsigned int m_Width, m_Height;
        };

        class Window_Move : public Event {
          public:
            Window_Move(int x, int y) : m_X(x), m_Y(y) {}

            inline int get_window_x() const {
                return m_X;
            }
            inline int get_Window_y() const {
                return m_Y;
            }

            std::string to_string() const override {
                std::stringstream ss;
                ss << "Window Move: " << m_X << ", " << m_Y;
                return ss.str();
            }
            EVENT_CLASS_TYPE(Window_Move)
            EVENT_CLASS_CATEGORY(Event_Category_Instance)
          private:
            int m_X, m_Y;
        };

        class Instance_Tick : public Event {
          public:
            Instance_Tick() = default;

            EVENT_CLASS_TYPE(Instance_Tick)
            EVENT_CLASS_CATEGORY(Event_Category_Instance)
        };

        class Instance_Update : public Event {
          public:
            Instance_Update() = default;

            EVENT_CLASS_TYPE(Instance_Update)
            EVENT_CLASS_CATEGORY(Event_Category_Instance)
        };

        class Instance_Render : public Event {
          public:
            Instance_Render() = default;

            EVENT_CLASS_TYPE(Instance_Render)
            EVENT_CLASS_CATEGORY(Event_Category_Instance)
        };

    } /* Event */
} /* Andromeda */
