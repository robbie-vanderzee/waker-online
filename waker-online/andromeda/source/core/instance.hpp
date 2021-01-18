#pragma once

#include "core.hpp"

int main(int argc, char const * argv[]);

namespace Andromeda {
    class Instance {
      public:
        Instance();
        virtual ~Instance();

        virtual int8_t start();

      private:
        static Instance * s_Instance;
        friend int main(int argc, char const * argv[]);
    };

    Instance * create_instance();
} /* Andromeda */
