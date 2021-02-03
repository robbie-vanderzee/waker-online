#include "waker-online.hpp"

namespace Waker {
    Instance::Instance(const std::string & name) : Andromeda::Instance(name) {
        ANDROMEDA_INFO("Initializing {0}", name);
        m_Instance_Name = name;
    }
    Instance::~Instance() {
    }
} /* Waker */
