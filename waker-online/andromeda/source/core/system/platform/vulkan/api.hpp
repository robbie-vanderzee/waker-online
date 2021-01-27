#pragma once

#include "core/graphics/api.hpp"

#include <vulkan/vulkan.h>

namespace Andromeda {
    namespace Graphics {
        namespace Vulkan {
            class API : public Andromeda::Graphics::API {
              public:
                virtual void initialize() override;
                virtual void process() override;
                virtual void shutdown() override;
            };
        } /* Vulkan */
    } /* Graphics */
} /* Andromeda */
