#include "renderer.hpp"

namespace Andromeda {
    namespace Graphics {
        std::unique_ptr<Graphics::API> Renderer::s_API = nullptr;

        void Renderer::initialize(Graphics::Info info, API::Type type) {
            s_API = Graphics::API::create_API(info, type);
            s_API->initialize();
        }

        void Renderer::process() {
            s_API->process();
        }

        void Renderer::shutdown() {
            s_API->shutdown();
        }
    } /* Graphics */
} /* Andromeda */
