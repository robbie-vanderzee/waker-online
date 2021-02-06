#include "core/instance.hpp"
#include "core/system/input/input.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Input {
        bool is_key_pressed(const std::unique_ptr<Window> & window, Code::Key key) {
            auto glfw_window = std::any_cast<GLFWwindow *> (window->get_native_window());
            auto state = glfwGetKey(glfw_window, static_cast<int>(key));
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

        bool is_mouse_button_pressed(const std::unique_ptr<Window> & window, Code::Mouse button) {
            auto glfw_window = std::any_cast<GLFWwindow *> (window->get_native_window());
            auto state = glfwGetMouseButton(glfw_window, static_cast<int>(button));
            return state == GLFW_PRESS;
        }

        Mouse::Position get_mouse_position(const std::unique_ptr<Window> & window) {
            auto glfw_window = std::any_cast<GLFWwindow *> (window->get_native_window());
            Mouse::Position position;
            glfwGetCursorPos(glfw_window, & position.x, & position.y);
            return position;
        }

        double get_mouse_x(const std::unique_ptr<Window> & window) {
            return get_mouse_position(window).x;
        }

        double get_mouse_y(const std::unique_ptr<Window> & window) {
            return get_mouse_position(window).y;
        }
    } /* Input */
} /* Andromeda */
