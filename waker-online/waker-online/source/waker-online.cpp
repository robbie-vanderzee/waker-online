#include "waker-online.hpp"
#include "test_layer.hpp"

#include <core/link.hpp>

namespace Waker {
    Instance::Instance(const std::string & name) : Andromeda::Instance(name) {
        ANDROMEDA_INFO("Initializing {0}", name);
        m_Instance_Name = name;
        push_layer(new Test_Layer());
    }
    Instance::~Instance() {

    }
} /* Waker */
