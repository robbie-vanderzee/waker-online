#pragma once

#include "core/system/interface/window/window.hpp"

#include <GLFW/glfw3.h>

namespace Andromeda {
    namespace Linux {
        class Window : public Andromeda::Window {
          public:
            Window(const Window_Properties& properties);
            virtual ~Window();

            void on_update() override;

            inline uint32_t get_width() const override {
                return m_Data.width;
            }
            inline uint32_t get_height() const override {
                return m_Data.height;
            }


            inline void set_event_callback(const Event_Callback_Function& callback) override {
                m_Data.Event_Callback = callback;
            }
            void set_v_sync(bool vsync_on) override;
            bool is_v_synced() const override;

            inline virtual void* get_native_window() const override {
                return m_Window;
            }

          private:
            virtual void init(const Window_Properties& properties);
            virtual void shutdown();
          private:
            GLFWwindow* m_Window;

            struct Window_Data {
                std::string title;
                uint32_t width, height;
                bool vsync_on;

                Event_Callback_Function Event_Callback;
            };

            Window_Data m_Data;
        };
    } /* Linux */
} /* Andromeda */
