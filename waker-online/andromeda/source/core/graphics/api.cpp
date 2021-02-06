#include "api.hpp"

#include "core/system/platform/vulkan/api.hpp"

namespace Andromeda {
    namespace Graphics {
        std::unique_ptr<API> API::create_API(Graphics::Info info, API::Type type) {
            switch (type) {
                case API::Type::None:
                    ANDROMEDA_CORE_ASSERT(false, "Graphics API <None> [Software] is unsupported.");
                    return nullptr;
                case API::Type::Vulkan:
                    return std::make_unique<Vulkan::API>(info);
            }
            ANDROMEDA_CORE_ASSERT(false, "Graphics API is undefined.");
            return nullptr;
        }
    } /* Graphics */
} /* Andromeda */
