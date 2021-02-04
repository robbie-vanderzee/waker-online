#pragma once

#include "core/graphics/context.hpp"
#include "core/system/interface/window/window.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Linux {
        class Window : public Andromeda::Window {
          public:
            Window(const Window_Properties & properties);

            virtual ~Window();

            void on_update() override;

            inline unsigned int get_width() const override {
                return m_Data.width;
            }
            inline unsigned int get_height() const override {
                return m_Data.height;
            }

            inline void set_event_callback(const Event_Callback_Function & callback) override {
                m_Data.Event_Callback = callback;
            }

            inline virtual std::any get_native_window() const override {
                return m_Window;
            }

          private:
            virtual void initialize(const Window_Properties & properties);
            virtual void shutdown();
          private:
            GLFWwindow * m_Window;

            struct Window_Data {
                std::string title;
                unsigned int width, height;

                Event_Callback_Function Event_Callback;
            };

            Window_Data m_Data;
        };
    } /* Linux */
} /* Andromeda */
