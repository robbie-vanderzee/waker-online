#include "window.hpp"

#include "core/graphics/renderer.hpp"
#include "core/system/events/event.hpp"
#include "core/system/events/instance.hpp"
#include "core/system/events/keyboard.hpp"
#include "core/system/events/mouse.hpp"

namespace Andromeda {
    namespace Linux {
        static unsigned int s_GLFW_Windows = 0;

        static void glfw_error_callback(int error, const char * message) {
            ANDROMEDA_CORE_ERROR("GLFW Error ({0}): {1}", error, message);
        }

        Window::Window(const Window_Properties & properties) {
            initialize(properties);
        }

        Window::~Window() {
            shutdown();
        }

        void Window::initialize(const Window_Properties & properties) {
            m_Data.title = properties.title;
            m_Data.width = properties.width;
            m_Data.height = properties.height;
            ANDROMEDA_CORE_INFO("Initializing window {0} ({1}, {2}).", m_Data.title, m_Data.width, m_Data.height);
            if(s_GLFW_Windows == 0) {
                int response = glfwInit();
                ANDROMEDA_CORE_ASSERT(response, "Failed to initialize GLFW.");
                glfwSetErrorCallback(glfw_error_callback);
            }
            switch(Graphics::Renderer::get_API()) {
                case Graphics::API_TYPE::None:
                    break;
                case Graphics::API_TYPE::Vulkan:
                    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
                    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
                    break;
            }
            m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), nullptr, nullptr);
            glfwSetWindowUserPointer(m_Window, & m_Data);
            s_GLFW_Windows++;
            glfwSetWindowSizeCallback(m_Window, [](GLFWwindow * window, int width, int height) {
                Window_Data & data = * (Window_Data *) glfwGetWindowUserPointer(window);
                data.width = width;
                data.height = height;
                Event::Window_Resize event(width, height);
                data.Event_Callback(event);
            });
            glfwSetWindowCloseCallback(m_Window, [](GLFWwindow * window) {
                Window_Data & data = * (Window_Data *) glfwGetWindowUserPointer(window);
                Event::Window_Close event;
                data.Event_Callback(event);
            });
            glfwSetKeyCallback(m_Window, [](GLFWwindow * window, int key, int /*scancode*/, int action, int /*mods*/) {
                Window_Data & data = * (Window_Data *) glfwGetWindowUserPointer(window);
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
                }
            });
            glfwSetCharCallback(m_Window, [](GLFWwindow * window, unsigned int key) {
                Window_Data & data = * (Window_Data *) glfwGetWindowUserPointer(window);
                Event::Key_Type event(static_cast<Key_Code>(key));
                data.Event_Callback(event);
            });
            glfwSetMouseButtonCallback(m_Window, [](GLFWwindow * window, int button, int action, int /*mods*/) {
                Window_Data & data = * (Window_Data *) glfwGetWindowUserPointer(window);
                switch(action) {
                    case GLFW_PRESS: {
                        Event::Mouse_Button_Press event((Mouse_Code) button);
                        data.Event_Callback(event);
                        break;
                    }
                    case GLFW_RELEASE: {
                        Event::Mouse_Button_Release event((Mouse_Code) button);
                        data.Event_Callback(event);
                        break;
                    }
                }
            });
            glfwSetScrollCallback(m_Window, [](GLFWwindow * window, double x_Offset,  double y_Offset) {
                Window_Data & data = * (Window_Data *) glfwGetWindowUserPointer(window);
                Event::Mouse_Scroll event(x_Offset, y_Offset);
                data.Event_Callback(event);
            });
            glfwSetCursorPosCallback(m_Window, [](GLFWwindow * window, double x_Position, double y_Position) {
                Window_Data & data = * (Window_Data *) glfwGetWindowUserPointer(window);
                Event::Mouse_Move event(x_Position, y_Position);
                data.Event_Callback(event);
            });
        }

        void Window::shutdown() {
            glfwDestroyWindow(m_Window);
            s_GLFW_Windows--;
            if(s_GLFW_Windows == 0) glfwTerminate();
        }

        void Window::on_update() {
            glfwPollEvents();
        }

        void Window::set_v_sync(bool vsync_on) {
            m_Data.vsync_on = vsync_on;
        }

        bool Window::is_v_synced() const {
            return m_Data.vsync_on;
        }

    } /* Linux */
} /* Andromeda */
