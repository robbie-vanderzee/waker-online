#pragma once

#include "core.hpp"

int main(int argc, char const * argv[]);

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

      private:
        friend int ::main(int argc, char const * argv[]);
    };

    std::unique_ptr<Instance> create_instance();
} /* Andromeda */
