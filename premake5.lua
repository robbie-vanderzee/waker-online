workspace "waker-online"
  configurations {
    "debug",
    "release"
  }

  flags {
    "MultiProcessorCompile"
  }

  binary_directory = "%{cfg.buildcfg}/%{cfg.system}"

  include_directories = {}
  include_directories["spdlog"] = "%{wks.location}/waker-online/includes/spdlog/include"
  include_directories["glfw"] = "%{wks.location}/waker-online/includes/glfw/include/GLFW"

  newoption {
    trigger = "logging",
    description = "Enable console logging."
  }

  configuration "logging"
    defines {
      "LOGGING"
    }

  filter "configurations:debug"
    defines {
      "DEBUG"
    }
    warnings "extra"
    runtime "debug"
    symbols "on"

  filter "configurations:release"
    defines {
      "RELEASE"
    }
    optimize "on"

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
