#pragma once

#include "core/core.hpp"

namespace Andromeda {
    namespace Event {
        enum class Type {
            None = 0,
            Tick, Update, Render,
            Connected, Disconnected,
            Move, Close, Resize, Refresh, Focus, Defocus, Maximize, Minimize, Restore,
            Press, Release, Type,
            Scroll
        };

        enum class Category : unsigned int {
            None        = 0,
            Instance    = BIT(0),
            Monitor     = BIT(1),
            Window      = BIT(2),
            Input       = BIT(3),
            Keyboard    = BIT(4),
            Mouse       = BIT(5),
            Controller  = BIT(6),
            Button      = BIT(7)
        };

        inline constexpr Category operator | (Category lhs, Category rhs) {
            using T = std::underlying_type_t <Category>;
            return static_cast<Category>(static_cast<T>(lhs) | static_cast<T>(rhs));
        }

        inline constexpr Category & operator |= (Category & lhs, Category rhs) {
            lhs = lhs | rhs;
            return lhs;
        }

        inline constexpr Category operator & (Category lhs, Category rhs) {
            using T = std::underlying_type_t <Category>;
            return static_cast<Category>(static_cast<T>(lhs) & static_cast<T>(rhs));
        }

        inline constexpr Category & operator &= (Category & lhs, Category rhs) {
            lhs = lhs & rhs;
            return lhs;
        }

#define EVENT_CLASS_TYPE(type) static Andromeda::Event::Type get_static_type() { return type; }\
    Andromeda::Event::Type get_event_type() const override { return get_static_type(); }\
    const char * get_event_name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual Andromeda::Event::Category get_category() const override { return category; }

        class Event {
            friend class Dispatcher;
          public:
            virtual Andromeda::Event::Type get_event_type() const = 0;
            virtual const char * get_event_name() const = 0;
            virtual Andromeda::Event::Category get_category() const = 0;
            virtual std::string to_string() const {
                return get_event_name();
            }
            inline constexpr bool is_in_category(Andromeda::Event::Category category) {
                using T = std::underlying_type_t <Andromeda::Event::Category>;
                return static_cast<T>(get_category() & category);
            }
            inline constexpr bool is_category(Andromeda::Event::Category category) {
                return get_category() == category;
            }
          public:
            bool consumed = false;
        };

        class Dispatcher {
          public:
            Dispatcher(Event & event) : m_Event(event) {
            }
            template<typename T, typename F>
            bool dispatch(const F & function) {
                if (m_Event.get_event_type() == T::get_static_type()) {
                    m_Event.consumed = function(static_cast<T &>(m_Event));
                    return true;
                }
                return false;
            }
          private:
            Event & m_Event;
        };

        inline std::ostream & operator<< (std::ostream & os, const Event & e) {
            return os << e.to_string();
        }

        using Callback = std::function<void (Event &)>;

    } /* Event */
} /* Andromeda */
