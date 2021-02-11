# andromeda::core::interface::event::

load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

cc_library(
    name = "andromeda_interface_event",
    hdrs = [
        "event.hpp",
        "events.hpp",
        "instance.hpp",
        "keyboard.hpp",
        "mouse.hpp",
    ],
    deps = [
        ":andromeda_core",
        ":andromeda_interface_input_codes",
    ],
)
