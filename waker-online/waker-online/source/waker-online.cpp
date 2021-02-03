#include "waker-online.hpp"

namespace Waker {
    Instance::Instance(const std::string & name) : Andromeda::Instance(name) {
        m_Instance_Name = name;
    }
    Instance::~Instance() {
    }
} /* Waker */
