#pragma once

#include "event.hpp"

#include "core/system/interface/input/code/key.hpp"

namespace Andromeda {
    namespace Event {
        namespace Keyboard {
            class Key : public Event {
              public:
                class Press;
                class Release;
                class Type;
              public:
                inline Input::Code::Key get_key_code() const {
                    return m_Key_Code;
                }

                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Input | Andromeda::Event::Category::Keyboard)
              protected:
                Key(Input::Code::Key key_code) : m_Key_Code(key_code) {}

                Input::Code::Key m_Key_Code;
            };

            class Key::Press : public Key {
              public:
                Press(Input::Code::Key key_code, unsigned int repeat_count) : Key(key_code), m_Repeat_Count(repeat_count) {}

                inline unsigned int get_repeat_count() const {
                    return m_Repeat_Count;
                }

                std::string to_string() const override {
                    return fmt::format("Key Press: {0} ({1} repeats)", m_Key_Code, m_Repeat_Count);
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Press)

              private:
                unsigned int m_Repeat_Count;
            };

            class Key::Release : public Key {
              public:
                Release(Input::Code::Key key_code) : Key(key_code) {}

                std::string to_string() const override {
                    return fmt::format("Key Release: {0}", m_Key_Code);
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Release)
            };

            class Key::Type : public Key {
              public:
                Type(Input::Code::Key key_code) : Key(key_code) {}

                std::string to_string() const override {
                    return fmt::format("Key Type: {0}", m_Key_Code);
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Type)
            };

        } /* Keyboard */
    } /* Event */
} /* Andromeda */
