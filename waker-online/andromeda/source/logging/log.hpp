#pragma once

#include "core/core.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace Andromeda {
    namespace Logging {
        class Log {
          public:
            static void initialize();
            static std::shared_ptr<spdlog::logger> & get_core_logger() {
                return s_Core_Logger;
            }
            static std::shared_ptr<spdlog::logger> & get_application_logger() {
                return s_Application_Logger;
            }
          private:
            static std::shared_ptr<spdlog::logger> s_Core_Logger;
            static std::shared_ptr<spdlog::logger> s_Application_Logger;
        };
    } /* Logging */
} /* Andromeda */
