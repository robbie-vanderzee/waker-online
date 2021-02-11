load("@bazel_skylib//lib:paths.bzl", "paths")
load("@bazel_tools//tools/build_defs/repo:utils.bzl", "maybe")

def configure_submodule_dependencies(waker_online_repository_alias = "@", waker_online_path = "./"):
    # self
    # TODO include <Andromeda> as module repository
    # includes
    maybe(
        native.new_local_repository,
        name = "glfw_headers",
        build_file = waker_online_repository_alias + "//:build/includes/glfw/headers/BUILD.overlay",
        path = paths.join(waker_online_path, "include/glfw"),
    )

    maybe(
        native.new_local_repository,
        name = "glm",
        build_file = waker_online_repository_alias + "//:build/includes/glm/BUILD.overlay",
        path = paths.join(waker_online_path, "include/glm"),
    )

    maybe(
        native.new_local_repository,
        name = "spdlog",
        build_file = waker_online_repository_alias + "//:build/includes/spdlog/BUILD.overlay",
        path = paths.join(waker_online_path, "include/vulkan_headers"),
    )

    maybe(
        native.new_local_repository,
        name = "vulkan_headers",
        build_file = waker_online_repository_alias + "//:build/includes/vulkan/headers/BUILD.overlay",
        path = paths.join(waker_online_path, "include/vulkan_headers"),
    )
