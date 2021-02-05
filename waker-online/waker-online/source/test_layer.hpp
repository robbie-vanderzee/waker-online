#pragma once

#include <andromeda.hpp>

class Test_Layer : public Andromeda::Layer {
  public:
    Test_Layer();
    virtual ~Test_Layer() = default;

    virtual void on_attach() override;
    virtual void on_detach() override;
    virtual void on_update() override;
    virtual void on_event(Andromeda::Event::Event & event) override;
};
