#pragma once

#include <andromeda.hpp>

namespace Waker {
    class Instance : public Andromeda::Instance {
      public:
        Instance(Andromeda::Instance::Info info);
        ~Instance() override;

        void run();
        void terminate();

        void on_event(Andromeda::Event::Event & event);

      private:
        bool on_window_close(Andromeda::Event::Window::Close & close);
        bool on_instance_terminate(Andromeda::Event::Instance::Terminate & terminate);
      private:
        std::shared_ptr<Andromeda::Window> m_Window;
        Andromeda::Layer::Stack m_Layers;
        Andromeda::Instance::Info m_Info;
    };
} /* Waker */

std::unique_ptr<Andromeda::Instance> Andromeda::create_instance() {
    Andromeda::Instance::Info info = {"Waker [ ~ ] Online", ANDROMEDA, true};
    return std::make_unique<Waker::Instance>(info);
}
