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


#ifdef LOGGING
// Core log macros
#define ANDROMEDA_CORE_TRACE(...)    ::Andromeda::Logging::Log::get_core_logger()->trace(__VA_ARGS__)
#define ANDROMEDA_CORE_INFO(...)     ::Andromeda::Logging::Log::get_core_logger()->info(__VA_ARGS__)
#define ANDROMEDA_CORE_WARN(...)     ::Andromeda::Logging::Log::get_core_logger()->warn(__VA_ARGS__)
#define ANDROMEDA_CORE_ERROR(...)    ::Andromeda::Logging::Log::get_core_logger()->error(__VA_ARGS__)
#define ANDROMEDA_CORE_CRITICAL(...) ::Andromeda::Logging::Log::get_core_logger()->critical(__VA_ARGS__)

// Applications log macros
#define ANDROMEDA_TRACE(...)         ::Andromeda::Logging::Log::get_application_logger()->trace(__VA_ARGS__)
#define ANDROMEDA_INFO(...)          ::Andromeda::Logging::Log::get_application_logger()->info(__VA_ARGS__)
#define ANDROMEDA_WARN(...)          ::Andromeda::Logging::Log::get_application_logger()->warn(__VA_ARGS__)
#define ANDROMEDA_ERROR(...)         ::Andromeda::Logging::Log::get_application_logger()->error(__VA_ARGS__)
#define ANDROMEDA_CRITICAL(...)      ::Andromeda::Logging::Log::get_application_logger()->critical(__VA_ARGS__)

#else

// Core log macros
#define ANDROMEDA_CORE_TRACE(...)
#define ANDROMEDA_CORE_INFO(...)
#define ANDROMEDA_CORE_WARN(...)
#define ANDROMEDA_CORE_ERROR(...)
#define ANDROMEDA_CORE_CRITICAL(...)

// Applications log macros
#define ANDROMEDA_TRACE(...)
#define ANDROMEDA_INFO(...)
#define ANDROMEDA_WARN(...)
#define ANDROMEDA_ERROR(...)
#define ANDROMEDA_CRITICAL(...)

#endif
