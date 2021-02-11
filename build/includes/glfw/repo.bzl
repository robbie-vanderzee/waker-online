"""
Repository rule to statically link against the Vulkan SDK.
Requires installing the GLFW from https://www.glfw.org/.
If GLFW is not installed, this generates an empty rule and you may
encounter linker errors like `error: undefined reference to 'glfwCreateWindow'`.
"""

def _impl(repository_ctx):
    if "GLFW" in repository_ctx.os.environ:
        path = repository_ctx.os.environ["GLFW"]
        repository_ctx.symlink(path, "glfw")
        repository_ctx.file("BUILD", """
cc_library(
    name = "sdk",
    srcs = select({
        "//conditions:default": [
            "glfw/lib/libglfw.so",
        ],
    }),
    visibility = ["//visibility:public"],
)""")
    else:
        # Empty rule. Will fail to link for just targets that use Vulkan.
        repository_ctx.file("BUILD", """
cc_library(
    name = "sdk",
    srcs = [],
    visibility = ["//visibility:public"],
)""")

glfw_setup = repository_rule(
    implementation = _impl,
    local = True,
)
