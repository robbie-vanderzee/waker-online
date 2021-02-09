#include "waker-online.hpp"
#include "test_layer.hpp"

#include <core/link.hpp>

namespace Waker {
    Instance::Instance(Instance::Info info) : m_Info(info) {
        ANDROMEDA_INFO("Initializing {0} | built with {1} /", m_Info.application, m_Info.engine);
        m_Window = Andromeda::Window::create_window(Andromeda::Window::Properties(
                       m_Info.application, {1920, 1080}, {0, 0}, Andromeda::Window::Option::Decorated | Andromeda::Window::Option::Visible));
        m_Window->set_event_callback(ANDROMEDA_BIND_FN(Instance::on_event));
        Andromeda::Graphics::Renderer::initialize({m_Info.application, m_Info.engine}, Andromeda::Graphics::API::Type::Vulkan);
        Andromeda::Graphics::Renderer::set_window_context(m_Window);
        Andromeda::Graphics::Renderer::process();
        m_Layers.push_layer(std::make_unique<Test_Layer>());
        Instance::run();
    }

    Instance::~Instance() {
        Andromeda::Graphics::Renderer::shutdown();
    }

    void Instance::run() {
        while (m_Info.active) {
            std::ranges::for_each(m_Layers, [](const auto & layer) {
                layer->on_update();
            });
            if (m_Window) m_Window->on_update();
        }
    }

    void Instance::terminate() {
        m_Info.active = false;
    }

    void Instance::on_event(Andromeda::Event::Event & event) {
#ifdef DEBUG
        ANDROMEDA_INFO(event);
#endif
        Andromeda::Event::Dispatcher dispatcher(event);
        dispatcher.dispatch<Andromeda::Event::Window::Close> (ANDROMEDA_BIND_FN(Instance::on_window_close));
        dispatcher.dispatch<Andromeda::Event::Instance::Terminate> (ANDROMEDA_BIND_FN(Instance::on_instance_terminate));

        std::ranges::find_if(std::rbegin(m_Layers), std::rend(m_Layers), [& event](const auto & layer) {
            layer->on_event(event);
            return event.consumed;
        });
    }

    bool Instance::on_window_close(Andromeda::Event::Window::Close &) {
        terminate();
        return true;
    }

    bool Instance::on_instance_terminate(Andromeda::Event::Instance::Terminate &) {
        terminate();
        return true;
    }
} /* Waker */
