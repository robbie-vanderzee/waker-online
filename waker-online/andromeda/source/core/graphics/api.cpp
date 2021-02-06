#include "api.hpp"

#include "core/system/platform/vulkan/api.hpp"

namespace Andromeda {
    namespace Graphics {
#ifdef VULKAN
        API::Type API::s_API = API::Type::Vulkan;
#endif
        std::unique_ptr<API> API::create_API() {
            switch (s_API) {
                case API::Type::None:
                    ANDROMEDA_CORE_ASSERT(false, "Graphics API <None> [Software] is unsupported.");
                    return nullptr;
                case API::Type::Vulkan:
                    return std::make_unique<Vulkan::API>();
            }
            ANDROMEDA_CORE_ASSERT(false, "Graphics API is undefined.");
            return nullptr;
        }
    } /* Graphics */
} /* Andromeda */
