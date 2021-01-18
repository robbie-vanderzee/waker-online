#pragma once

#include "includes.hpp"

/* ------- Logging ---------------------------------------------------------- */
/* ----------------- Logic -------------------------------------------------- */
#ifdef LOGGING
#define ANDROMEDA_ASSERT(x, ...) { if(!(x)) { ANDROMEDA_ERROR("Assertion Failure: {0}", __VA_ARGS__); }}
#define ANDROMEDA_CORE_ASSERT(x, ...) {if(!(x)) { ANDROMEDA_CORE_ERROR("Assertion Failure: {0}", __VA_ARGS__); }}
#else
#define ANDROMEDA_ASSERT(x, ...)
#define ANDROMEDA_CORE_ASSERT(x, ...)
#endif
/* ----------------- end of: Logic ------------------------------------------ */
/* ------- end of: Logging -------------------------------------------------- */
