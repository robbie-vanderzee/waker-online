#include "test.hpp"

namespace Waker {
    namespace Layer {
        Test::Test() {

        }

        Test::~Test() {

        }

        void Test::on_attach() {
            ANDROMEDA_INFO("Attaching test layer");
        }

        void Test::on_detach() {
            ANDROMEDA_INFO("Removing test layer");
        }

        void Test::on_update() {

        }

        void Test::on_event(Andromeda::Event::Event &) {

        }
    } /* Layer */
} /* Waker */
