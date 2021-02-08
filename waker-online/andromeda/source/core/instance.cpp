#include "instance.hpp"

namespace Andromeda {

    Instance::Instance(const std::string & application) : m_Info({application, ANDROMEDA, true}) {
        ANDROMEDA_CORE_INFO(m_Info.application);
        m_Window = Window::create_window(Window::Properties(m_Info.application, {1920, 1080}, {0, 0}, Window::Option::Decorated | Window::Option::Visible));
        m_Window->set_event_callback(ANDROMEDA_BIND_FN(Instance::on_event));
        Graphics::Renderer::initialize(Graphics::Info{m_Info.application, m_Info.engine}, Graphics::API::Type::Vulkan);
        Graphics::Renderer::set_window_context(m_Window);
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
        while (m_Info.active) {
            std::ranges::for_each(m_Layer_Stack, [](const auto layer) {
                layer->on_update();
            });
            m_Window->on_update();
        }
        return EXIT_SUCCESS;
    }

    void Instance::terminate() {
        m_Info.active = false;
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
