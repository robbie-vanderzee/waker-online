#pragma once

#include "includes.hpp"

/* Macros ------------------------------------------------------------------- */
/* ------- Titles ----------------------------------------------------------- */
#define ANDROMEDA "ANDROMEDA"
/* ------- end of: Titles --------------------------------------------------- */
/* ------- Numerics --------------------------------------------------------- */
#define BIT(x) ( 1 << x )
/* ------- end of: Numerics ------------------------------------------------- */
/* ------- Structure -------------------------------------------------------- */
#define ANDROMEDA_BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1)
/* -------------- Concepts -------------------------------------------------- */
/* -------------- end of: Concepts ------------------------------------------ */
/* ------- end of: Structure ------------------------------------------------ */
/* ------- Logging ---------------------------------------------------------- */
/* ----------------- Logic -------------------------------------------------- */
#ifdef LOGGING
#define ANDROMEDA_ASSERT(x, ...) { if(!(x)) { ANDROMEDA_ERROR("Assertion Failure: {0}", __VA_ARGS__); }}
#define ANDROMEDA_CORE_ASSERT(x, ...) {if(!(x)) { ANDROMEDA_CORE_ERROR("Assertion Failure: {0}", __VA_ARGS__); }}

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
#define ANDROMEDA_ASSERT(x, ...)
#define ANDROMEDA_CORE_ASSERT(x, ...)

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
/* ----------------- end of: Logic ------------------------------------------ */
/* ------- end of: Logging -------------------------------------------------- */
/* end of: Macros ----------------------------------------------------------- */
