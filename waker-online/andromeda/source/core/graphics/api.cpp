#include "api.hpp"

#include "core/system/platform/vulkan/api.hpp"

namespace Andromeda {
    namespace Graphics {
#ifdef VULKAN
        API_TYPE API::s_API = API_TYPE::Vulkan;
#endif
        std::unique_ptr<API> API::create_API() {
            switch (s_API) {
                case API_TYPE::None:
                    ANDROMEDA_CORE_ASSERT(false, "Graphics API <NONE> is unsupported.");
                    return nullptr;
                case API_TYPE::Vulkan:
                    return std::make_unique<Vulkan::API>();
            }
            ANDROMEDA_CORE_ASSERT(false, "Graphics API is undefined.");
            return nullptr;
        }
    } /* Graphics */
} /* Andromeda */
