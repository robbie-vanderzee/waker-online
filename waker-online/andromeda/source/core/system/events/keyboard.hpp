#pragma once

#include "event.hpp"
#include "core/system/input/codes/key_code.hpp"

namespace Andromeda {
    namespace Event {
        class Key_Event : public Event {
          public:
            inline Key_Code get_key_code() const {
                return m_Key_Code;
            }

            EVENT_CLASS_CATEGORY(Event_Category_Input | Event_Category_Keyboard)

          protected:
            Key_Event(Key_Code key_code) : m_Key_Code(key_code) {}

            Key_Code m_Key_Code;
        };

        class Key_Press : public Key_Event {
          public:
            Key_Press(Key_Code key_code, unsigned int repeat_count) : Key_Event(key_code), m_Repeat_Count(repeat_count) {}

            inline unsigned int get_repeat_count() const {
                return m_Repeat_Count;
            }

            std::string to_string() const override {
                std::stringstream ss;
                ss << "Key Press: " << m_Key_Code << " (" << m_Repeat_Count << " repeats)";
                return ss.str();
            }

            EVENT_CLASS_TYPE(Key_Press)

          private:
            unsigned int m_Repeat_Count;
        };

        class Key_Release : public Key_Event {
          public:
            Key_Release(Key_Code key_code) : Key_Event(key_code) {}

            std::string to_string() const override {
                std::stringstream ss;
                ss << "Key Release: " << m_Key_Code;
                return ss.str();
            }

            EVENT_CLASS_TYPE(Key_Release)
        };

        class Key_Type : public Key_Event {
          public:
            Key_Type(Key_Code key_code) : Key_Event(key_code) {}

            std::string to_string() const override {
                std::stringstream ss;
                ss << "Key Type: " << m_Key_Code;
                return ss.str();
            }

            EVENT_CLASS_TYPE(Key_Type)
        };


    } /* Event */
} /* Andromeda */
