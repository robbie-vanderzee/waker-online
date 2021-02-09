#pragma once

#include <andromeda.hpp>

class Test_Layer : public Andromeda::Layer {
  public:
    Test_Layer();
    ~Test_Layer() override;

    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_event(Andromeda::Event::Event & event) override;
};
