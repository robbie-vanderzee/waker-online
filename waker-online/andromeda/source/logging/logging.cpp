#include "logging.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Andromeda {
    namespace Logging {
        std::shared_ptr<spdlog::logger> Log::s_Core_Logger;
        std::shared_ptr<spdlog::logger> Log::s_Application_Logger;

        void Log::initialize() {
            std::vector<spdlog::sink_ptr> log_sinks;

            log_sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
            log_sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt> ("logging/andromeda.log", true));

            log_sinks[0]->set_pattern("%^[%T] %n [%l]: %v%$");
            log_sinks[1]->set_pattern("[%T] [%l] %n: %v");

            s_Core_Logger = std::make_shared<spdlog::logger> ("Andromeda", begin(log_sinks), end(log_sinks));
            spdlog::register_logger(s_Core_Logger);
            s_Core_Logger->set_level(spdlog::level::trace);
            s_Core_Logger->flush_on(spdlog::level::trace);

            s_Application_Logger = std::make_shared<spdlog::logger> ("Waker [ ~ ] Online", begin(log_sinks), end(log_sinks));
            spdlog::register_logger(s_Application_Logger);
            s_Application_Logger->set_level(spdlog::level::trace);
            s_Application_Logger->flush_on(spdlog::level::trace);

            spdlog::set_default_logger(s_Core_Logger);
        }
    } /* Logging */
} /* Andromeda */
