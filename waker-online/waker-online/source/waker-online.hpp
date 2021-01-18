#pragma once

#include <andromeda.hpp>

namespace Waker {
    class Waker : public ::Andromeda::Instance {
      public:
        Waker();
        ~Waker();
        int8_t start();

    };
} /* Waker */

Andromeda::Instance * Andromeda::create_instance() {
    return new Waker::Waker();
}
