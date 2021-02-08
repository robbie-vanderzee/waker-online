#pragma once

#include "core/system/interface/display/window.hpp"

#include "monitor.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Linux {
        class Window : public Andromeda::Window {
          public:
            Window(const Window::Properties & properties);

            virtual ~Window();

            void on_update() override;

            inline Window::Viewport get_viewport() const override {
                return m_Data.viewport;
            }

            inline Window::Position get_position() const override {
                return m_Data.position;
            }

            // Input State
            virtual bool is_key_pressed(Andromeda::Input::Code::Key key) override;
            virtual bool is_mouse_button_pressed(Andromeda::Input::Code::Mouse button) override;
            virtual Andromeda::Input::Mouse::Position get_mouse_position() override;

            // Attributes
            virtual void set_fullscreen() override;
            virtual void set_attributes(Window::Option options) override;
            inline void set_event_callback(const Event::Callback & callback) override {
                m_Data.callback = callback;
            }

            inline virtual std::any get_native_window() const override {
                return m_Window;
            }

          private:
            void initialize(const Window::Properties & properties);
            void create_fullscreen_window(Andromeda::Linux::Monitor & monitor);
            void create_windowed_window();
            void set_callbacks();
            void shutdown();

          private:
            inline static std::size_t s_GLFW_Windows = 0;

            GLFWwindow * m_Window;

            struct Data {
                std::string title;
                Window::Viewport viewport;
                Window::Position position;
                Window::Option options;
                Event::Callback callback;
            };

            Window::Data m_Data;
        };
    } /* Linux */
} /* Andromeda */
