#pragma once

#include "core/core.hpp"
#include "core/system/interface/event/event.hpp"

namespace Andromeda {
    class Layer {
      public:
        class Stack;
      public:
        virtual ~Layer() = default;

        virtual void on_attach() = 0;
        virtual void on_detach() = 0;
        virtual void on_update() = 0;
        virtual void on_event(Event::Event &) = 0;
    };
} /* Andromeda */
