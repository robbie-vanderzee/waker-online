#pragma once

#include "core.hpp"

#include "core/graphics/renderer.hpp"

#include "core/system/event/event.hpp"
#include "core/system/event/instance.hpp"

#include "core/system/interface/layer/stack.hpp"
#include "core/system/interface/window/window.hpp"


int main(int argc, char const * argv[]);

namespace Andromeda {
    class Instance {
      public:
        Instance(const std::string & name = "Andromeda /instance");
        virtual ~Instance();

        void push_layer(Layer * layer);
        void pop_layer(Layer * layer);

        void push_overlay(Layer * overlay);
        void pop_overlay(Layer * overlay);

        int run();

        void terminate();
        void on_event(Event::Event & e);

      private:
        bool on_window_close(Event::Window::Close & event);

      private:
        std::shared_ptr<Window> m_Window;
        Layer::Stack m_Layer_Stack;
        std::unique_ptr<Graphics::Renderer> m_Renderer;

        struct Info {
            std::string application;
            std::string engine;
            bool active;
        };

        Info m_Info;
      private:
        friend int ::main(int argc, char const * argv[]);
    };

    std::unique_ptr<Instance> create_instance();
} /* Andromeda */
