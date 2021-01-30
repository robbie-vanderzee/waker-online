#include "core/instance.hpp"
#include "core/system/input/input.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    bool Input::is_key_pressed(Key_Code key) {
        auto window = static_cast<GLFWwindow *>(Instance::get_instance().get_window().get_native_window());
        auto state = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::is_mouse_button_pressed(Mouse_Code button) {
        auto window = static_cast<GLFWwindow *>(Instance::get_instance().get_window().get_native_window());
        auto state = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::get_mouse_position() {
        auto window = static_cast<GLFWwindow *>(Instance::get_instance().get_window().get_native_window());
        double x_Position, y_Position;
        glfwGetCursorPos(window, & x_Position, & y_Position);
        return { (float)x_Position, (float)y_Position };
    }

    float Input::get_mouse_x() {
        auto[x, y] = get_mouse_position();
        return x;
    }

    float Input::get_mouse_y() {
        auto[x, y] = get_mouse_position();
        return y;
    }
} /* Andromeda */
