#pragma once

#include "core.hpp"
#include "instance.hpp"

extern std::unique_ptr<Andromeda::Instance> Andromeda::create_instance();

int main(int argc, char const * argv[]) {
#ifdef LOGGING
    Andromeda::Logging::Log::initialize();
    for (int i = 0; i < argc; i++) ANDROMEDA_CORE_TRACE("{0}", argv[i]);
#endif
    ANDROMEDA_CORE_INFO("");
    ANDROMEDA_CORE_TRACE("starting Andromeda /");
    ANDROMEDA_CORE_INFO("");
    auto instance = Andromeda::create_instance();
    ANDROMEDA_CORE_INFO("");
    ANDROMEDA_CORE_TRACE("terminating Andromeda /");
    ANDROMEDA_CORE_INFO("");
    return EXIT_SUCCESS;
}
