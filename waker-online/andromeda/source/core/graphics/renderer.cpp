#include "renderer.hpp"
#include "3d/renderer3d.hpp"

namespace Andromeda {
    namespace Graphics {
        void Renderer::initialize() {
            Render::initialize();
            Renderer3D::initialize();
        }

        void Renderer::shutdown() {
            Renderer3D::shutdown();
        }
    } /* Graphics */
} /* Andromeda */
