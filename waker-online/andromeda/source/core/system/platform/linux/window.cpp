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
            if (s_GLFW_Windows == 0) glfwTerminate();
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
            auto position = s_Monitors[0].get_position();
            auto mode = s_Monitors[0].get_video_mode();
            auto monitor = std::any_cast<GLFWmonitor *>(s_Monitors[0].get_native_monitor());
            glfwSetWindowMonitor(m_Window, monitor, position.x, position.y, mode.width, mode.height, mode.refresh_rate);
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

            ANDROMEDA_CORE_INFO("Initializing window {0} ({1}, {2}).", m_Data.title, m_Data.viewport.width, m_Data.viewport.height);
            if (s_GLFW_Windows == 0) {
                unsigned int response = glfwInit();
                ANDROMEDA_CORE_ASSERT(response, "Failed to initialize GLFW.");
                glfwSetErrorCallback(glfw_error_callback);
                int count = 0;
                GLFWmonitor ** monitors = glfwGetMonitors(& count);
                Andromeda::Linux::Monitor::s_GLFW_Monitors = count;
                for (std::size_t i = 0; i < Andromeda::Linux::Monitor::s_GLFW_Monitors; i++) s_Monitors.emplace_back(monitors[i], i == 0);
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, m_Data.options == Option::Resizable);
            glfwWindowHint(GLFW_DECORATED, m_Data.options == Option::Decorated);
            glfwWindowHint(GLFW_VISIBLE, m_Data.options == Option::Visible);
            glfwWindowHint(GLFW_FLOATING, m_Data.options == Option::Floating);

            m_Window = glfwCreateWindow(m_Data.viewport.width, m_Data.viewport.height, m_Data.title.c_str(), nullptr, nullptr);
            glfwSetWindowUserPointer(m_Window, & m_Data);
            s_GLFW_Windows++;

            set_callbacks();
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
        }

    } /* Linux */
} /* Andromeda */
