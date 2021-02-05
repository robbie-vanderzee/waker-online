#include "core/instance.hpp"
#include "core/system/input/input.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Input {
        bool is_key_pressed(Code::Key key) {
            auto window = std::any_cast<GLFWwindow *> (Instance::get_instance()->get_window()->get_native_window());
            auto state = glfwGetKey(window, static_cast<int>(key));
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

        bool is_mouse_button_pressed(Code::Mouse button) {
            auto window = std::any_cast<GLFWwindow *> (Instance::get_instance()->get_window()->get_native_window());
            auto state = glfwGetMouseButton(window, static_cast<int>(button));
            return state == GLFW_PRESS;
        }

        Mouse_Position get_mouse_position() {
            auto window = std::any_cast<GLFWwindow *> (Instance::get_instance()->get_window()->get_native_window());
            Mouse_Position position;
            glfwGetCursorPos(window, & position.x, & position.y);
            return position;
        }

        double get_mouse_x() {
            return get_mouse_position().x;
        }

        double get_mouse_y() {
            return get_mouse_position().y;
        }
    } /* Input */
} /* Andromeda */
