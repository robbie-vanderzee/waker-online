#pragma once

#include "core.hpp"

#include "core/system/events/event.hpp"
#include "core/system/events/instance.hpp"

#include "core/system/interface/layer/layer_stack.hpp"
#include "core/system/interface/window/window.hpp"

int main(int argc, char const * argv[]);

namespace Andromeda {
    class Instance {
      public:
        Instance(const std::string & name = "Andromeda /instance");
        virtual ~Instance();

        Instance(const Instance &) = delete;
        Instance & operator= (const Instance &) = delete;

        void push_layer(Layer * layer);
        void pop_layer(Layer * layer);

        void push_overlay(Layer * overlay);
        void pop_overlay(Layer * overlay);

        int run();

        void terminate();
        void on_event(Event::Event & e);

      public:

        inline static Instance * get_instance() {
            return s_Instance;
        }

        inline std::shared_ptr<Window> get_window() {
            return m_Window;
        }

        inline std::string get_instance_name() {
            return m_Instance_Name;
        }

        void set_v_sync(bool v_sync);

      private:
        bool on_window_close(Event::Window_Close & e);

      private:
        bool m_Running = true;
        std::string m_Instance_Name;
        std::shared_ptr<Window> m_Window;
        Layer_Stack m_Layer_Stack;
      private:
        static Instance * s_Instance;
        friend int ::main(int argc, char const * argv[]);
    };

    Instance * create_instance();
} /* Andromeda */
