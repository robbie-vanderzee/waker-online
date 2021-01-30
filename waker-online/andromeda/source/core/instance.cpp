#include "instance.hpp"

#include "core/graphics/renderer.hpp"

namespace Andromeda {
    Instance * Instance::s_Instance = nullptr;

    Instance::Instance(const std::string & name) {
        s_Instance = this;
        m_Instance_Name = name;
        ANDROMEDA_CORE_INFO(m_Instance_Name);
        m_Window = Window::create_window(Window_Properties(m_Instance_Name));
        m_Window->set_event_callback(ANDROMEDA_BIND_FN(Instance::on_event));
        Graphics::Renderer::initialize();
    }

    Instance::~Instance() {
        Graphics::Renderer::shutdown();
    }


    int Instance::run() {
        while(m_Running) {
            m_Window->on_update();
        }
        return EXIT_SUCCESS;
    }

    void Instance::on_event(Event::Event & e) {
#ifdef DEBUG
        ANDROMEDA_CORE_INFO(e);
#endif
        Event::Dispatcher dispatcher(e);
        dispatcher.dispatch<Event::Window_Close>(ANDROMEDA_BIND_FN(Instance::on_window_close));
    }

    bool Instance::on_window_close(Event::Window_Close &) {
        m_Running = false;
        return true;
    }
} /* Andromeda */
