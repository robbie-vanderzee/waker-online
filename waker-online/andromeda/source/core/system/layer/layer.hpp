#pragma once

#include "core/core.hpp"
#include "core/system/interface/event/event.hpp"

namespace Andromeda {
    class Layer {
      public:
        class Stack;
      public:
        Layer(const std::string & name = "Layer");
        virtual ~Layer() = default;

        virtual void on_attach() {}
        virtual void on_detach() {}
        virtual void on_update() {}
        virtual void on_event(Event::Event &) {}

        const std::string & get_name() const {
            return m_Name;
        }
      private:
        std::string m_Name;
    };
} /* Andromeda */
