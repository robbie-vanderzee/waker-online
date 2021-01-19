#pragma once

#include "includes.hpp"

/* Macros ------------------------------------------------------------------- */
/* ------- Numerics --------------------------------------------------------- */
#define BIT(x) ( 1 << x )
/* ------- end of: Numerics ------------------------------------------------- */
/* ------- Structure -------------------------------------------------------- */
#define ANDROMEDA_BIND_FN(fn) std::bind(&fn, this, std::placeholders::_1)
/* ------- end of: Structure ------------------------------------------------ */
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
/* end of: Macros ----------------------------------------------------------- */
