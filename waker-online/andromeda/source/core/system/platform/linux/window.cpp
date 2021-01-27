#include "window.hpp"

#include "core/system/events/event.hpp"
#include "core/system/events/instance.hpp"
#include "core/system/events/keyboard.hpp"
#include "core/system/events/mouse.hpp"


namespace Andromeda {
    namespace Linux {
        static bool s_GLFW_Initialized = false;

        static void glfw_error_callback(int error, const char* message) {
            ANDROMEDA_CORE_ERROR("GLFW Error ({0}): {1}", error, message);
        }

        Window::Window(const Window_Properties &properties) {
            init(properties);
        }

        Window::~Window() {
            shutdown();
        }

        void Window::init(const Window_Properties& properties) {
            m_Data.title = properties.title;
            m_Data.width = properties.width;
            m_Data.height = properties.height;

            ANDROMEDA_CORE_INFO("Initializing window {0} ({1}, {2})", m_Data.title, m_Data.width, m_Data.height);

            if(!s_GLFW_Initialized) {
                int32_t response = glfwInit();
                ANDROMEDA_CORE_ASSERT(response, "Could not initialize GLFW!");
                glfwSetErrorCallback(glfw_error_callback);
                s_GLFW_Initialized = true;
            }

            m_Window = glfwCreateWindow((int32_t)m_Data.width, (int32_t)m_Data.height, m_Data.title.c_str(), nullptr, nullptr);
            glfwMakeContextCurrent(m_Window);
            glfwSetWindowUserPointer(m_Window, &m_Data);

            glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int32_t width, int32_t height) {
                Window_Data &data = *(Window_Data*)glfwGetWindowUserPointer(window);
                data.width = width;
                data.height = height;

                Event::Window_Resize event(width, height);
                data.Event_Callback(event);
            });

            glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
                Window_Data& data = *(Window_Data*)glfwGetWindowUserPointer(window);
                Event::Window_Close event;
                data.Event_Callback(event);
            });

            glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int32_t key, int32_t /*scancode*/, int32_t action, int32_t /*mods*/) {
                Window_Data &data = *(Window_Data*)glfwGetWindowUserPointer(window);

                switch(action) {
                case GLFW_PRESS: {
                    Event::Key_Press event(static_cast<Key_Code>(key), 0);
                    data.Event_Callback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    Event::Key_Release event(static_cast<Key_Code>(key));
                    data.Event_Callback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    Event::Key_Press event(static_cast<Key_Code>(key), 1);
                    data.Event_Callback(event);
                    break;
                }
                default: {
                    ANDROMEDA_CORE_INFO("Unhandled key action {0}", action);
                    break;
                }
                }
            });

            glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t key) {
                Window_Data &data = *(Window_Data*)glfwGetWindowUserPointer(window);

                Event::Key_Type event(static_cast<Key_Code>(key));
                data.Event_Callback(event);
            });

            glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int32_t button, int32_t action, int32_t /*mods*/) {
                Window_Data &data = *(Window_Data*)glfwGetWindowUserPointer(window);

                switch(action) {
                case GLFW_PRESS: {
                    Event::Mouse_Button_Press event((Mouse_Code)button);
                    data.Event_Callback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    Event::Mouse_Button_Release event((Mouse_Code)button);
                    data.Event_Callback(event);
                    break;
                }
                default: {
                    ANDROMEDA_CORE_INFO("Unhandled mouse action {0}", action);
                    break;
                }
                }
            });

            glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double x_Offset,  double y_Offset) {
                Window_Data &data = *(Window_Data*)glfwGetWindowUserPointer(window);

                Event::Mouse_Scroll event((float)x_Offset, (float)y_Offset);
                data.Event_Callback(event);
            });

            glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double x_Position, double y_Position) {
                Window_Data &data = *(Window_Data*)glfwGetWindowUserPointer(window);

                Event::Mouse_Move event((float)x_Position, (float)y_Position);
                data.Event_Callback(event);
            });
        }

        void Window::shutdown() {
            glfwDestroyWindow(m_Window);
        }

        void Window::on_update() {
            glfwPollEvents();
        }

        void Window::set_v_sync(bool vsync_on) {
            if(vsync_on)
                glfwSwapInterval(1);
            else
                glfwSwapInterval(0);

            m_Data.vsync_on = vsync_on;
        }

        bool Window::is_v_synced() const {
            return m_Data.vsync_on;
        }

    } /* Linux */
} /* Andromeda */
