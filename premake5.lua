workspace "waker-online"
  toolset "clang"
  flags {
    "MultiProcessorCompile"
  }

  configurations {
    "debug",
    "release"
  }

  binary_directory = "%{cfg.buildcfg}/%{cfg.system}"

  include_directories = {}
  include_directories["spdlog"] = "%{wks.location}/waker-online/includes/spdlog/include"
  include_directories["glfw"] = "%{wks.location}/waker-online/includes/glfw/include/GLFW"
  include_directories["glm"] = "%{wks.location}/waker-online/includes/glm"
  include_directories["shaderc"] = "%{wks.location}/waker-online/includes/shaderc/libshaderc/include"

  newoption {
    trigger = "sanitize",
    value= "modules",
    description = "Enable sanitization runtimes.",
    allowed = {
      { "address", "AddressSantiizer" }
    }
  }

  filter "options:sanitize=address"
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
