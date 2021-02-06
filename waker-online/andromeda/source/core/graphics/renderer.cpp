#include "renderer.hpp"

namespace Andromeda {
    namespace Graphics {
        Renderer::Renderer(Graphics::Info info, API::Type type) : m_Info(info) {
            m_API = API::create_API(info, type);
        }

        Renderer::~Renderer() {

        }

        void Renderer::initialize() {
            m_API->initialize();
        }

        void Renderer::process() {
            m_API->process();
        }

        void Renderer::shutdown() {
            m_API->shutdown();
        }
    } /* Graphics */
} /* Andromeda */
