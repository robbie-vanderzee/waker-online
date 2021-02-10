#pragma once

#include "core/system/interface/display/monitor.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Linux {
        class Monitor : public Andromeda::Monitor {
            friend class Window;
          public:
            Monitor(GLFWmonitor * monitor, bool primary);

            void update() override;
            // Attributes
            inline void set_event_callback(const Event::Callback & callback) override {
                m_Data.callback = callback;
            };
            inline std::any get_native_monitor() const override {
                return m_Monitor;
            }

          private:
            inline static std::size_t s_GLFW_Monitors = 0;
            inline static std::vector<Monitor> s_Monitors;
            GLFWmonitor * m_Monitor;

            struct Data {
                std::string model;
                bool primary;
                Monitor::Position position;
                Monitor::Area area;
                Monitor::Mode mode;
                Event::Callback callback;
            };

            Monitor::Data m_Data;
        };
    } /* Linux */
} /* Andromeda */
