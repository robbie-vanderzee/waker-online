# andromeda::linux::interface::display::

load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

cc_library(
    name = "andromeda_linux_interface_display_monitor",
    srcs = [
        "monitor.cpp",
    ],
    hdrs = [
        "monitor.hpp",
    ],
    deps = [
        ":andromeda_core",
        ":andromeda_interface_event",
        ":glfw",
    ],
)

cc_library(
    name = "andromeda_linux_interface_display_window",
    srcs = [
        "window.cpp",
    ],
    hdrs = [
        "window.hpp",
    ],
    deps = [
        ":andromeda_core",
        ":andromeda_interface_display_window",
        ":andromeda_interface_event",
        ":andromeda_interface_input_manager",
        ":glfw",
    ],
)
