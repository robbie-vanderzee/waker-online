#include "command.hpp"

namespace Andromeda {
    namespace Graphics {
        std::unique_ptr<API> Command::s_API = API::create_API();
    } /* Graphics */
} /* Andromeda */
