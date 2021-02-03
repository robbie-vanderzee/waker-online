#include "context.hpp"

#include "renderer.hpp"
#include "core/system/platform/vulkan/context.hpp"

namespace Andromeda {
    namespace Graphics {
        std::unique_ptr<Graphics::Context> Graphics::Context::create_context(std::shared_ptr<Window> window) {
            switch(Renderer::get_API()) {
                case API_TYPE::None:
                    ANDROMEDA_CORE_ASSERT(false, "Renderer API <None> is unsupported.");
                    return nullptr;
                case API_TYPE::Vulkan:
                    return std::make_unique<Vulkan::Context> (window);
            }
            ANDROMEDA_CORE_ASSERT(false, "Renderer API is undefined.");
            return nullptr;
        }
    } /* Graphics */
} /* Andromeda */
