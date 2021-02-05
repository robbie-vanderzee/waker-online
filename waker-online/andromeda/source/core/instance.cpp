#include "instance.hpp"

#include "core/graphics/command.hpp"
#include "core/graphics/renderer.hpp"

namespace Andromeda {

    Instance * Instance::s_Instance = nullptr;

    Instance::Instance(const std::string & name) {
        s_Instance = std::move(this);
        m_Instance_Name = name;
        ANDROMEDA_CORE_INFO(m_Instance_Name);
        m_Window = Window::create_window(Window_Properties(m_Instance_Name));
        m_Window->set_event_callback(ANDROMEDA_BIND_FN(Instance::on_event));
        Graphics::Renderer::initialize();
        Graphics::Renderer::Command::set_window_context(m_Window);
        Graphics::Renderer::process();
    }

    Instance::~Instance() {
        Graphics::Renderer::shutdown();
    }

    void Instance::push_layer(Layer * layer) {
        m_Layer_Stack.push_layer(layer);
    }

    void Instance::pop_layer(Layer * layer) {
        m_Layer_Stack.pop_layer(layer);
    }

    void Instance::push_overlay(Layer * overlay) {
        m_Layer_Stack.push_overlay(overlay);
    }

    void Instance::pop_overlay(Layer * overlay) {
        m_Layer_Stack.pop_overlay(overlay);
    }

    int Instance::run() {
        while (m_Running) {
            for (auto & layer : m_Layer_Stack) layer->on_update();
            m_Window->on_update();
        }
        return EXIT_SUCCESS;
    }

    void Instance::terminate() {
        m_Running = false;
    }

    void Instance::on_event(Event::Event & e) {
#ifdef DEBUG
        ANDROMEDA_CORE_INFO(e);
#endif
        Event::Dispatcher dispatcher(e);
        dispatcher.dispatch<Event::Window_Close> (ANDROMEDA_BIND_FN(Instance::on_window_close));

        for (auto iterator = m_Layer_Stack.rbegin(); iterator != m_Layer_Stack.rend(); ++iterator) {
            if (e.consumed) break;
            (*iterator)->on_event(e);
        }
    }

    bool Instance::on_window_close(Event::Window_Close &) {
        terminate();
        return true;
    }
} /* Andromeda */
