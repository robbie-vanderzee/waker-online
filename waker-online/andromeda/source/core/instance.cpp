#include "instance.hpp"

namespace Andromeda {
    Instance * Instance::s_Instance = nullptr;

    Instance::Instance(const std::string& name) {
        s_Instance = this;
        m_Instance_Name = name;
        m_Window = Window::create(Window_Properties(name));
        m_Window->set_event_callback(ANDROMEDA_BIND_FN(Instance::on_event));
    }

    Instance::~Instance() {

    }


    int8_t Instance::run() {
        while(m_Running) {
            m_Window->on_update();
        }
        return EXIT_SUCCESS;
    }

    void Instance::on_event(Event::Event & e) {
        ANDROMEDA_CORE_INFO(e);
        Event::Dispatcher dispatcher(e);
        dispatcher.dispatch<Event::Window_Close>(ANDROMEDA_BIND_FN(Instance::on_window_close));
    }

    bool Instance::on_window_close(Event::Window_Close &e) {
        m_Running = false;
        return true;
    }
} /* Andromeda */
