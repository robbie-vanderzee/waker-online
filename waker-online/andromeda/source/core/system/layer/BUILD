# andromeda::core::system::layer::

load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

cc_library(
    name = "andromeda_system_layer",
    hdrs = [
        "layer.hpp",
    ],
    deps = [
        ":andromeda_core",
        ":andromeda_interface_event",
    ],
)

cc_library(
    name = "andromeda_system_layer_stack",
    srcs = [
        "stack.cpp",
    ],
    hdrs = [
        "stack.hpp",
    ],
    deps = [
        ":andromeda_core",
        ":andromeda_system_layer",
    ],
)
