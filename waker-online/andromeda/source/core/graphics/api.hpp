#pragma once

#include "core/core.hpp"
#include "core/system/interface/window/window.hpp"

namespace Andromeda {
    namespace Graphics {
        struct Info {
            const std::string application;
            const std::string engine;
        };
        class API {
          public:
            enum class Type {
                None = 0,
                Vulkan = 1
            };
          public:
            virtual ~API() = default;

            virtual void initialize() = 0;
            virtual void process() = 0;
            virtual void shutdown() = 0;

            virtual void set_window_context(std::weak_ptr<Window> window) = 0;

            virtual Type get_API_Type() const = 0;

            static std::unique_ptr<API> create_API(Graphics::Info info, API::Type type);
        };
    } /* Graphics */
} /* Andromeda */
