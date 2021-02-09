#include "window.hpp"

#include "core/system/interface/event/event.hpp"
#include "core/system/interface/event/instance.hpp"
#include "core/system/interface/event/keyboard.hpp"
#include "core/system/interface/event/mouse.hpp"

namespace Andromeda {
    namespace Linux {
        static void glfw_error_callback(int error, const char * message) {
            ANDROMEDA_CORE_ERROR("GLFW Error ({0}): {1}", error, message);
        }

        Window::Window(const Window::Properties & properties) {
            initialize(properties);
        }

        Window::~Window() {
            shutdown();
        }

        void Window::on_update() {
            glfwPollEvents();
        }

        bool Window::is_key_pressed(Andromeda::Input::Code::Key key) {
            auto state = glfwGetKey(m_Window, static_cast<int>(key));
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

        bool Window::is_mouse_button_pressed(Andromeda::Input::Code::Mouse button) {
            auto state = glfwGetMouseButton(m_Window, static_cast<int>(button));
            return state == GLFW_PRESS;
        }

        Andromeda::Input::Mouse::Position Window::get_mouse_position() {
            Andromeda::Input::Mouse::Position position;
            glfwGetCursorPos(m_Window, & position.x, & position.y);
            return position;
        }

        void Window::set_fullscreen() {
            auto monitor = std::any_cast<GLFWmonitor *>(std::begin(Monitor::s_Monitors)->get_native_monitor());
            Monitor::Data & data = * (Monitor::Data *) glfwGetMonitorUserPointer(monitor);
            glfwSetWindowMonitor(m_Window, monitor, data.position.x, data.position.y, data.mode.width, data.mode.height, data.mode.refresh_rate);
        }

        void Window::set_attributes(Window::Option options) {
            m_Data.options = options;
            glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, m_Data.options == Option::Resizable);
            glfwSetWindowAttrib(m_Window, GLFW_DECORATED, m_Data.options == Option::Decorated);
            glfwSetWindowAttrib(m_Window, GLFW_VISIBLE, m_Data.options == Option::Visible);
            glfwSetWindowAttrib(m_Window, GLFW_FLOATING, m_Data.options == Option::Floating);
        }

        void Window::initialize(const Window::Properties & properties) {
            m_Data.title = properties.title;
            m_Data.viewport = properties.viewport;
            m_Data.position = properties.position;
            m_Data.options = properties.options;

            if (s_GLFW_Windows == 0) {
                unsigned int response = glfwInit();
                ANDROMEDA_CORE_ASSERT(response, "Failed to initialize GLFW.");
                glfwSetErrorCallback(glfw_error_callback);
                int count = 0;
                GLFWmonitor ** monitors = glfwGetMonitors(& count);
                Monitor::s_GLFW_Monitors = count;
                Monitor::s_Monitors.clear();
                Monitor::s_Monitors.reserve(Monitor::s_GLFW_Monitors);
                for (std::size_t i = 0; i < Monitor::s_GLFW_Monitors; i++) Monitor::s_Monitors.emplace(std::begin(Monitor::s_Monitors) + i, monitors[i], i == 0);
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, m_Data.options == Option::Resizable);
            glfwWindowHint(GLFW_DECORATED, m_Data.options == Option::Decorated);
            glfwWindowHint(GLFW_VISIBLE, m_Data.options == Option::Visible);
            glfwWindowHint(GLFW_FLOATING, m_Data.options == Option::Floating);

            if (m_Data.options == Option::Fullscreen) create_fullscreen_window(*std::begin(Monitor::s_Monitors));
            else create_windowed_window();

            glfwSetWindowUserPointer(m_Window, & m_Data);
            s_GLFW_Windows++;
            set_callbacks();
        }

        void Window::create_fullscreen_window(Andromeda::Linux::Monitor & monitor) {
            auto native = std::any_cast<GLFWmonitor *>(monitor.get_native_monitor());
            Monitor::Data & data = * (Monitor::Data *) glfwGetMonitorUserPointer(native);
            glfwWindowHint(GLFW_REFRESH_RATE, data.mode.refresh_rate);
            ANDROMEDA_CORE_INFO("Initializing fullscreen window {0} [{1} x {2}]", m_Data.title, data.mode.width, data.mode.height);
            m_Window = glfwCreateWindow(data.mode.width, data.mode.height, m_Data.title.c_str(), native, nullptr);
        }

        void Window::create_windowed_window() {
            ANDROMEDA_CORE_INFO("Initializing window {0} [{1} x {2}]", m_Data.title, m_Data.viewport.width, m_Data.viewport.height);
            m_Window = glfwCreateWindow(m_Data.viewport.width, m_Data.viewport.height, m_Data.title.c_str(), nullptr, nullptr);
        }

        void Window::set_callbacks() {
            glfwSetWindowPosCallback(m_Window, [](GLFWwindow * window, int x, int y) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                data.position.x = x;
                data.position.y = y;
                Event::Window::Move event(x, y);
                data.callback(event);
            });

            glfwSetWindowSizeCallback(m_Window, [](GLFWwindow * window, int width, int height) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                data.viewport.width = width;
                data.viewport.height = height;
                Event::Window::Resize event(width, height);
                data.callback(event);
            });

            glfwSetWindowCloseCallback(m_Window, [](GLFWwindow * window) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                Event::Window::Close event;
                data.callback(event);
            });

            glfwSetWindowRefreshCallback(m_Window, [](GLFWwindow * window) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                Event::Window::Refresh event;
                data.callback(event);
            });

            glfwSetWindowFocusCallback(m_Window, [](GLFWwindow * window, int focused) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                switch (focused) {
                    case GLFW_TRUE: {
                        Event::Window::Focus event;
                        data.callback(event);
                        break;
                    }
                    case GLFW_FALSE: {
                        Event::Window::Defocus event;
                        data.callback(event);
                        break;
                    }
                }
            });

            glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow * window, int maximized) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                switch (maximized) {
                    case GLFW_TRUE: {
                        Event::Window::Maximize event;
                        data.callback(event);
                        break;
                    }
                    case GLFW_FALSE: {
                        Event::Window::Restore event;
                        data.callback(event);
                        break;
                    }
                }
            });

            glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow * window, int iconified) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                switch (iconified) {
                    case GLFW_TRUE: {
                        Event::Window::Minimize event;
                        data.callback(event);
                        break;
                    }
                    case GLFW_FALSE: {
                        Event::Window::Restore event;
                        data.callback(event);
                        break;
                    }
                }
            });

            glfwSetKeyCallback(m_Window, [](GLFWwindow * window, int key, int /*scancode*/, int action, int /*mods*/) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                switch (action) {
                    case GLFW_PRESS: {
                        Event::Keyboard::Key::Press event(static_cast<Andromeda::Input::Code::Key>(key), 0);
                        data.callback(event);
                        break;
                    }
                    case GLFW_RELEASE: {
                        Event::Keyboard::Key::Release event(static_cast<Andromeda::Input::Code::Key>(key));
                        data.callback(event);
                        break;
                    }
                    case GLFW_REPEAT: {
                        Event::Keyboard::Key::Press event(static_cast<Andromeda::Input::Code::Key>(key), 1);
                        data.callback(event);
                        break;
                    }
                }
            });

            glfwSetCharCallback(m_Window, [](GLFWwindow * window, unsigned int key) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                Event::Keyboard::Key::Type event(static_cast<Andromeda::Input::Code::Key>(key));
                data.callback(event);
            });

            glfwSetMouseButtonCallback(m_Window, [](GLFWwindow * window, int button, int action, int /*mods*/) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                switch (action) {
                    case GLFW_PRESS: {
                        Event::Mouse::Button::Press event((Andromeda::Input::Code::Mouse) button);
                        data.callback(event);
                        break;
                    }
                    case GLFW_RELEASE: {
                        Event::Mouse::Button::Release event((Andromeda::Input::Code::Mouse) button);
                        data.callback(event);
                        break;
                    }
                }
            });

            glfwSetScrollCallback(m_Window, [](GLFWwindow * window, double x_Offset,  double y_Offset) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                Event::Mouse::Scroll event(x_Offset, y_Offset);
                data.callback(event);
            });

            glfwSetCursorPosCallback(m_Window, [](GLFWwindow * window, double x_Position, double y_Position) {
                Window::Data & data = * (Window::Data *) glfwGetWindowUserPointer(window);
                Event::Mouse::Move event(x_Position, y_Position);
                data.callback(event);
            });
        }

        void Window::shutdown() {
            glfwDestroyWindow(m_Window);
            s_GLFW_Windows--;
            if (s_GLFW_Windows == 0) glfwTerminate();
        }

    } /* Linux */
} /* Andromeda */
