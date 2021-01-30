#include "waker-online.hpp"

namespace Waker {
    Waker::Waker(const std::string & name) : Andromeda::Instance(name) {
        m_Instance_Name = name;
    }
    Waker::~Waker() {

    }
} /* Waker */
