#include "instance.hpp"

#include "core/graphics/command.hpp"
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
            std::ranges::for_each(std::begin(m_Layer_Stack), std::end(m_Layer_Stack), [](const auto layer) {
                layer->on_update();
            });
            m_Window->on_update();
        }
        return EXIT_SUCCESS;
    }

    void Instance::terminate() {
        m_Running = false;
    }

    void Instance::on_event(Event::Event & event) {
#ifdef DEBUG
        ANDROMEDA_CORE_INFO(event);
#endif
        Event::Dispatcher dispatcher(event);
        dispatcher.dispatch<Event::Window::Close> (ANDROMEDA_BIND_FN(Instance::on_window_close));

        std::ranges::find_if(std::rbegin(m_Layer_Stack), std::rend(m_Layer_Stack), [& event](const auto layer) {
            layer->on_event(event);
            return event.consumed;
        });
    }

    bool Instance::on_window_close(Event::Window::Close &) {
        terminate();
        return true;
    }
} /* Andromeda */
