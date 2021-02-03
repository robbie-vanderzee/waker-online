#pragma once

#include "core/core.hpp"

namespace Andromeda {
    namespace Event {
        enum class EVENT_TYPE {
            None = 0,
            Window_Close, Window_Resize, Window_Focus, Window_Unfocused, Window_Move,
            Instance_Tick, Instance_Update, Instance_Render,
            Key_Press, Key_Release, Key_Type,
            Mouse_Button_Press, Mouse_Button_Release, Mouse_Move, Mouse_Scroll
        };

        enum EVENT_CATEGORY {
            None                        = 0,
            Event_Category_Instance     = BIT(0),
            Event_Category_Input        = BIT(1),
            Event_Category_Keyboard     = BIT(2),
            Event_Category_Mouse        = BIT(3),
            Event_Category_Controller   = BIT(4)
        };

// Event type instantiation macro
#define EVENT_CLASS_TYPE(type) static EVENT_TYPE get_static_type() { return EVENT_TYPE::type; }\
    virtual EVENT_TYPE get_event_type() const override { return get_static_type(); }\
    virtual const char * get_event_name() const override { return #type; }
// Event category instantiation macro
#define EVENT_CLASS_CATEGORY(category) virtual unsigned int get_category_flags() const override { return category; }

        class Event {
            friend class Dispatcher;
          public:
            virtual EVENT_TYPE get_event_type() const = 0;
            virtual const char * get_event_name() const = 0;
            virtual unsigned int get_category_flags() const = 0;
            virtual std::string to_string() const {
                return get_event_name();
            }
            inline bool is_in_category(EVENT_CATEGORY category) {
                return get_category_flags() & category;
            }
          public:
            bool active = false;
        };

        class Dispatcher {
          public:
            Dispatcher(Event & event) : m_Event(event) {
            }
            template<typename T, typename F>
            bool dispatch(const F & function) {
                if(m_Event.get_event_type() == T::get_static_type()) {
                    m_Event.active = function(static_cast<T &>(m_Event));
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

    } /* Event */
} /* Andromeda */
