#pragma once

#include "event.hpp"

namespace Andromeda {
    namespace Event {
        namespace Monitor {
            class Connected : public Event {
              public:
                Connected() = default;

                std::string to_string() const override {
                    return fmt::format("Monitor Connected");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Connected)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance | Andromeda::Event::Category::Monitor)
            };
            class Disconnected : public Event {
              public:
                Disconnected() = default;

                std::string to_string() const override {
                    return fmt::format("Monitor Disconnected");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Disconnected)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance | Andromeda::Event::Category::Monitor)
            };
        } /* Monitor */

        namespace Window {
            class Move : public Event {
              public:
                Move(int x, int y) : m_X(x), m_Y(y) {}

                inline int get_window_x() const {
                    return m_X;
                }
                inline int get_window_y() const {
                    return m_Y;
                }

                std::string to_string() const override {
                    return fmt::format("Window Move: {0}, {1}", m_X, m_Y);
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Move)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance | Andromeda::Event::Category::Window)
              private:
                int m_X, m_Y;
            };
            class Resize : public Event {
              public:
                Resize(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

                inline unsigned int get_window_width() const {
                    return m_Width;
                }
                inline unsigned int get_window_height() const {
                    return m_Height;
                }

                std::string to_string() const override {
                    return fmt::format("Window Resize: {0}, {1}", m_Width, m_Height);
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Resize)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance | Andromeda::Event::Category::Window)
              private:
                unsigned int m_Width, m_Height;
            };
            class Close : public Event {
              public:
                Close() = default;

                std::string to_string() const override {
                    return fmt::format("Window Close");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Close)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance | Andromeda::Event::Category::Window)
            };
            class Refresh : public Event {
              public:
                Refresh() = default;

                std::string to_string() const override {
                    return fmt::format("Window Refresh");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Refresh)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance | Andromeda::Event::Category::Window)
            };
            class Focus : public Event {
              public:
                Focus() = default;

                std::string to_string() const override {
                    return fmt::format("Window Focus");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Focus)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance | Andromeda::Event::Category::Window)
            };
            class Defocus : public Event {
              public:
                Defocus() = default;

                std::string to_string() const override {
                    return fmt::format("Window Defocus");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Defocus)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance | Andromeda::Event::Category::Window)
            };
            class Maximize : public Event {
              public:
                Maximize() = default;

                std::string to_string() const override {
                    return fmt::format("Window Maximize");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Maximize)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance | Andromeda::Event::Category::Window)
            };
            class Minimize : public Event {
              public:
                Minimize() = default;

                std::string to_string() const override {
                    return fmt::format("Window Minimize");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Minimize)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance | Andromeda::Event::Category::Window)
            };
            class Restore : public Event {
              public:
                Restore() = default;

                std::string to_string() const override {
                    return fmt::format("Window Restore");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Restore)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance | Andromeda::Event::Category::Window)
            };
        } /* Window */

        namespace Instance {
            class Tick : public Event {
              public:
                Tick() = default;

                std::string to_string() const override {
                    return fmt::format("Instance Tick");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Tick)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance)
            };

            class Update : public Event {
              public:
                Update() = default;

                std::string to_string() const override {
                    return fmt::format("Instance Update");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Update)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance)
            };

            class Render : public Event {
              public:
                Render() = default;

                std::string to_string() const override {
                    return fmt::format("Instance Render");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Render)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance)
            };

            class Terminate : public Event {
              public:
                Terminate() = default;

                std::string to_string() const override {
                    return fmt::format("Instance Terminate");
                }

                EVENT_CLASS_TYPE(Andromeda::Event::Type::Terminate)
                EVENT_CLASS_CATEGORY(Andromeda::Event::Category::Instance)
            };
        } /* Instance */
    } /* Event */
} /* Andromeda */
