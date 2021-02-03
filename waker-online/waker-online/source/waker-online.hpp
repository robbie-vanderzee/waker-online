#pragma once

#include <andromeda.hpp>

namespace Waker {
    class Instance : public Andromeda::Instance {
      public:
        Instance(const std::string & name = "Waker [ ~ ]");
        ~Instance();
      private:
        std::string m_Instance_Name;
    };
} /* Waker */

Andromeda::Instance * Andromeda::create_instance() {
    return new Waker::Instance("Waker [ ~ ] Online");
}
