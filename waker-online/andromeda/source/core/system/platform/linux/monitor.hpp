#pragma once

#include "core/system/interface/display/monitor.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Linux {
        class Monitor : public Andromeda::Monitor {
          public:
            inline static std::size_t s_GLFW_Monitors = 0;
          public:
            Monitor(GLFWmonitor * monitor, bool primary);

            virtual ~Monitor();

            virtual void update() override;

            inline virtual Monitor::Position get_position() const override {
                return m_Data.position;
            }

            inline virtual Monitor::Area get_area() const override {
                return m_Data.area;
            }

            inline virtual Monitor::Mode get_video_mode() const override {
                return m_Data.mode;
            }

            inline virtual bool is_primary() const override {
                return m_Data.primary;
            }
            // Attributes
            inline virtual void set_event_callback(const Event::Callback & callback) override {
                m_Data.callback = callback;
            };
            inline virtual std::any get_native_monitor() const override {
                return m_Monitor;
            }
          private:
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
