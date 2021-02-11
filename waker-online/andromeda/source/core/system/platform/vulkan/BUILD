# andromeda::vulkan::graphics::

load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

cc_library(
    name = "andromeda_vulkan_graphics_api",
    srcs = [
        "api.cpp",
    ],
    hdrs = [
        "api.hpp",
    ],
    deps = [
        ":andromeda_core",
        ":andromeda_interface_event",
        ":glfw",
    ],
)

cc_library(
    name = "andromeda_vulkan_graphics_renderer",
    srcs = [
        "context.cpp",
    ],
    hdrs = [
        "context.hpp",
    ],
    deps = [
        ":andromeda_core",
        ":andromeda_graphics_renderer",
        ":andromeda_interface_display_window",
        ":glfw",
        ":vulkan",
    ],
)
