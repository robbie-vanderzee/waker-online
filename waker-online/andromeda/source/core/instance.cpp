#include "instance.hpp"

namespace Andromeda {
    Instance * Instance::s_Instance = nullptr;

    Instance::Instance() {
        s_Instance = this;
    }

    Instance::~Instance() {

    }

    int8_t Instance::start() {
        return EXIT_SUCCESS;
    }
} /* Andromeda */
