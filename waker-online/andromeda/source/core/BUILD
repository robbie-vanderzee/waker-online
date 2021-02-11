# andromeda::core::

load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

cc_library(
    name = "andromeda_core",
    hdrs = [
        "core.hpp",
        "instance.hpp",
    ],
    deps = [
        ":andromeda_includes",
        ":andromeda_logging",
    ],
)

cc_library(
    name = "andromeda_link",
    hdrs = [
        "link.hpp",
    ],
    deps = [
        ":andromeda_core",
    ],
)
