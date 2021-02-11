# andromeda::

load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

cc_library(
    name = "andromeda",
    hdrs = [
        "andromeda.hpp",
    ],
    deps = [
        ":andromeda_core",
        ":andromeda_graphics_api",
        ":andromeda_graphics_renderer",
        ":andromeda_interface_display_monitor",
        ":andromeda_interface_display_window",
        ":andromeda_interface_event",
        ":andromeda_interface_input_code",
        ":andromeda_interface_input_manager",
        ":andromeda_link",
        ":andromeda_logging",
    ],
)

cc_library(
    name = "andromeda_includes",
    hdrs = [
        "includes.hpp",
    ],
)
