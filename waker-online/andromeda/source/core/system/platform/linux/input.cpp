#include "core/instance.hpp"
#include "core/system/input/input.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    bool Input::is_key_pressed(Key_Code key) {
        auto window = static_cast<GLFWwindow *>(Instance::get_instance().get_window().get_native_window());
        auto state = glfwGetKey(window, static_cast<int>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::is_mouse_button_pressed(Mouse_Code button) {
        auto window = static_cast<GLFWwindow *>(Instance::get_instance().get_window().get_native_window());
        auto state = glfwGetMouseButton(window, static_cast<int>(button));
        return state == GLFW_PRESS;
    }

    std::pair<double, double> Input::get_mouse_position() {
        auto window = static_cast<GLFWwindow *>(Instance::get_instance().get_window().get_native_window());
        double x_Position, y_Position;
        glfwGetCursorPos(window, & x_Position, & y_Position);
        return { x_Position, y_Position };
    }

    double Input::get_mouse_x() {
        auto[x, y] = get_mouse_position();
        return x;
    }

    double Input::get_mouse_y() {
        auto[x, y] = get_mouse_position();
        return y;
    }
} /* Andromeda */
