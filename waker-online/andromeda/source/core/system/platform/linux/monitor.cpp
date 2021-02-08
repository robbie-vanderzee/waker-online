#include "monitor.hpp"

#include "core/system/interface/event/event.hpp"
#include "core/system/interface/event/instance.hpp"

namespace Andromeda {
    namespace Linux {
        Monitor::Monitor(GLFWmonitor * monitor, bool primary) : m_Monitor(monitor) {
            m_Data.primary = primary;
            update();
            glfwSetMonitorUserPointer(m_Monitor, & m_Data);
            glfwSetMonitorCallback([](GLFWmonitor * monitor, int event) {
                Monitor::Data & data = * (Monitor::Data *) glfwGetMonitorUserPointer(monitor);
                switch (event) {
                    case GLFW_CONNECTED: {
                        Event::Monitor::Connected event;
                        data.callback(event);
                        break;
                    }
                    case GLFW_DISCONNECTED: {
                        Event::Monitor::Disconnected event;
                        data.callback(event);
                        break;
                    }
                }
            });
        }

        Monitor::~Monitor() {

        }

        void Monitor::update() {
            m_Data.model = std::string(glfwGetMonitorName(m_Monitor));
            glfwGetMonitorPos(m_Monitor,  & m_Data.position.x, & m_Data.position.y);
            glfwGetMonitorWorkarea(m_Monitor, & m_Data.area.x, & m_Data.area.y, & m_Data.area.width, & m_Data.area.height);
            const GLFWvidmode* mode = glfwGetVideoMode(m_Monitor);
            m_Data.mode = {mode->redBits, mode->greenBits, mode->blueBits, mode->refreshRate, mode->width, mode->height};
            ANDROMEDA_CORE_INFO("Monitor {0}, ({11}, {12}) [{1} x {2}] @ {3} Hz | RGB {4}::{5}::{6} | Space ({7}, {8}) [{9}, {10}]",
                                m_Data.model, m_Data.mode.width, m_Data.mode.height, m_Data.mode.refresh_rate,
                                m_Data.mode.red, m_Data.mode.green, m_Data.mode.blue,
                                m_Data.area.x, m_Data.area.y, m_Data.area.width, m_Data.area.height,
                                m_Data.position.x, m_Data.position.y);
        }

    } /* Linux */
} /* Andromeda */
