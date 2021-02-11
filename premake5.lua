workspace "waker-online"
  toolset "clang"
  flags {
    "LinkTimeOptimization"
  }

  configurations {
    "debug",
    "release"
  }

  binary_directory = "%{cfg.buildcfg}/%{cfg.system}"

  include_directories = {}
  include_directories["spdlog"] = "%{wks.location}/includes/spdlog/include"
  include_directories["glfw"] = "%{wks.location}/includes/glfw/include/GLFW"
  include_directories["glm"] = "%{wks.location}/includes/glm"
  include_directories["shaderc"] = "%{wks.location}/includes/shaderc/libshaderc/include"

  newoption {
    trigger = "sanitize",
    value = "sanitizer",
    description = "Enable sanitization runtimes.",
    allowed = {
      { "address", "AddressSantiizer" }
    }
  }

  filter "options:sanitize=address"
    toolset "gcc"
    buildoptions "-fsanitize=address -fno-omit-frame-pointer -fsanitize-address-use-after-scope"
    links {
      "asan"
    }

  newoption {
    trigger = "logging",
    description = "Enable console logging."
  }

  filter "options:logging"
    defines {
      "LOGGING"
    }

  newoption {
    trigger = "gfxapi",
    value = "API",
    description = "Select graphics API",
    default = "vulkan",
    allowed = {
      { "vulkan", "Vulkan" }
    }
  }

  filter "options:gfxapi=vulkan"
    defines {
      "VULKAN"
    }
    links {
      "vulkan"
    }

  filter "configurations:debug"
    defines {
      "DEBUG"
    }
    warnings "Extra"
    runtime "Debug"
    symbols "On"

  filter "configurations:release"
    defines {
      "RELEASE"
    }
    warnings "Extra"
    runtime "Release"
    optimize "Speed"

  filter "system:linux"
    defines {
      "LINUX"
    }
    links {
      "glfw"
    }

  include "waker-online/andromeda"
  include "waker-online/waker-online"

  buildcommands {
    "make %{cfg.buildcfg}"
  }

  cleancommands {
    "make clean %{cfg.buildcfg}"
  }
