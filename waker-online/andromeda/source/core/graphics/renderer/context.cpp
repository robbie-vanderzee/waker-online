#include "context.hpp"

#include "core/system/platform/vulkan/context.hpp"

namespace Andromeda {
    namespace Graphics {
        std::unique_ptr<Graphics::Renderer::Context> Graphics::Renderer::Context::create_context(API::Type type, std::shared_ptr<Window> window) {
            switch (type) {
                case API::Type::None:
                    ANDROMEDA_CORE_ASSERT(false, "Renderer API <None> is unsupported.");
                    return nullptr;
                case API::Type::Vulkan:
                    return std::make_unique<Vulkan::Context> (window);
            }
            ANDROMEDA_CORE_ASSERT(false, "Renderer API is undefined.");
            return nullptr;
        }
    } /* Graphics */
} /* Andromeda */
