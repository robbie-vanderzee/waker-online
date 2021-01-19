#pragma once

#include <andromeda.hpp>

namespace Waker {
    class Waker : public ::Andromeda::Instance {
      public:
        Waker(const std::string & name = "Waker Online [ ~ ]");
        ~Waker();
      private:
        std::string m_Instance_Name;
    };
} /* Waker */

Andromeda::Instance * Andromeda::create_instance() {
    return new Waker::Waker();
}
