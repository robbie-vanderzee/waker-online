#pragma once

#include <andromeda.hpp>

namespace Waker {
    namespace Layer {
        class Test : public Andromeda::Layer {
          public:
            Test();
            ~Test() override;

            void on_attach() override;
            void on_detach() override;
            void on_update() override;
            void on_event(Andromeda::Event::Event & event) override;
        };
    } /* Layer */
} /* Waker */
