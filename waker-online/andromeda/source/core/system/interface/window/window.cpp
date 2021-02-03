#include "window.hpp"

#ifdef LINUX
#include "core/system/platform/linux/window.hpp"
#endif

namespace Andromeda {
    std::shared_ptr<Window> Window::create_window(const Window_Properties & properties) {
#ifdef LINUX
        return std::make_shared<Linux::Window> (properties);
#else
        ANDROMEDA_CORE_ASSERT(false, "Unsupported window platform.");
        return nullptr;
#endif
    }
} /* Andromeda */
