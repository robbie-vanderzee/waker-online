#include "test_layer.hpp"

Test_Layer::Test_Layer() {

}

Test_Layer::~Test_Layer() {

}

void Test_Layer::on_attach() {
    ANDROMEDA_INFO("Attaching test layer");
}

void Test_Layer::on_detach() {
    ANDROMEDA_INFO("Removing test layer");
}

void Test_Layer::on_update() {

}

void Test_Layer::on_event(Andromeda::Event::Event &) {

}
