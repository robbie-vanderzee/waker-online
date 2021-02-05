#include "core/instance.hpp"
#include "core/system/input/input.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    bool Input::is_key_pressed(Key_Code key) {
        auto window = std::any_cast<GLFWwindow *> (Instance::get_instance()->get_window()->get_native_window());
        auto state = glfwGetKey(window, static_cast<int>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::is_mouse_button_pressed(Mouse_Code button) {
        auto window = std::any_cast<GLFWwindow *> (Instance::get_instance()->get_window()->get_native_window());
        auto state = glfwGetMouseButton(window, static_cast<int>(button));
        return state == GLFW_PRESS;
    }

    Input::Mouse_Position Input::get_mouse_position() {
        auto window = std::any_cast<GLFWwindow *> (Instance::get_instance()->get_window()->get_native_window());
        Mouse_Position position;
        glfwGetCursorPos(window, & position.x, & position.y);
        return position;
    }

    double Input::get_mouse_x() {
        return get_mouse_position().x;
    }

    double Input::get_mouse_y() {
        return get_mouse_position().y;
    }
} /* Andromeda */
