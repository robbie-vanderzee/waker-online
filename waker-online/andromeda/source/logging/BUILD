# andromeda::logging::

load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

cc_library(
    name = "andromeda_logging",
    srcs = [
        "log.cpp",
    ],
    hdrs = [
        "log.hpp",
    ],
    deps = [
        ":spdlog",
    ],
)
