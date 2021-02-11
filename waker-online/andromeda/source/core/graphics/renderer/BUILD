# andromeda::core::graphics::renderer

load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

cc_library(
    name = "andromeda_graphics_renderer",
    srcs = [
        "command.cpp",
        "context.cpp",
        "renderer.cpp",
    ],
    hdrs = [
        "command.hpp",
        "context.hpp",
        "renderer.hpp",
    ],
    deps = [
        ":andromeda_core",
        ":andromeda_interface_display_window",
        ":andromeda_logging",
    ],
)
