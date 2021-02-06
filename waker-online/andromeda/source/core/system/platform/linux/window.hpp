#pragma once

#include "core/graphics/context.hpp"
#include "core/system/interface/window/window.hpp"

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

            inline unsigned int get_width() const override {
                return m_Data.viewport.width;
            }

            inline unsigned int get_height() const override {
                return m_Data.viewport.height;
            }

            inline void set_event_callback(const Event_Callback_Function & callback) override {
                m_Data.Event_Callback = callback;
            }

            inline virtual std::any get_native_window() const override {
                return m_Window;
            }

          private:
            virtual void initialize(const Window::Properties & properties);
            virtual void shutdown();
          private:
            GLFWwindow * m_Window;

            struct Window_Data {
                std::string title;
                Window::Viewport viewport;
                Window::Position position;

                Event_Callback_Function Event_Callback;
            };

            Window_Data m_Data;
        };
    } /* Linux */
} /* Andromeda */
