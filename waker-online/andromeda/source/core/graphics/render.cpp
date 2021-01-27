#include "render.hpp"

namespace Andromeda {
    namespace Graphics {
        std::unique_ptr<API> Render::s_API = API::create_API();
    } /* Graphics */
} /* Andromeda */
