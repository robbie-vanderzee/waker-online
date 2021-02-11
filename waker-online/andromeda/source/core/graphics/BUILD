# andromeda::core::graphics::

load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

cc_library(
    name = "andromeda_graphics_api",
    srcs = "api.cpp",
    hdrs = "api.hpp",
    deps = [
        ":andromeda_interface_display_window",
        ":andromeda_vulkan_graphics_api",
    ],
)
