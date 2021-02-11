load("@rules_cc//cc:defs.bzl", "cc_library")

package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

# Helper for getting linkopts into libraries in a way we can easily spot and
# rewrite from bazel-to-cmake and copybara.
cc_library(
    name = "default_linkopts",
    linkopts = select({
        "//conditions:default": [
            "-ldl",
            "-lpthread",
        ],
    }),
)
