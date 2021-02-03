#include "command.hpp"

namespace Andromeda {
    namespace Graphics {
        namespace Renderer {
            std::unique_ptr<API> Command::s_API = API::create_API();
        } /* Renderer */
    } /* Graphics */
} /* Andromeda */
