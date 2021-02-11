# waker::core::

load("@rules_cc//cc:defs.bzl", "cc_binary")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

cc_binary(
    name = "waker_core",
    srcs = [
        "waker-online.cpp",
    ],
    hdrs = [
        "waker-online.hpp",
    ],
    deps = [
        ":andromeda",
        ":waker_layer_test",
    ],
)
