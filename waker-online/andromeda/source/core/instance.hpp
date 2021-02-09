#pragma once

#include "core.hpp"

namespace Andromeda {
    class Instance {
      public:
        struct Info {
            std::string application;
            std::string engine;
            bool active;
        };
      public:
        virtual ~Instance() = default;
    };

    std::unique_ptr<Instance> create_instance();
} /* Andromeda */
