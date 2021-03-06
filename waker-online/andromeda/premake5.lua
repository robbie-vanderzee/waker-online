project "andromeda"
  kind "StaticLib"
  language "C++"
  cppdialect "C++20"

  targetdir ( "binary/" ..binary_directory.. "/%{prj.name}" )
  objdir ("binary/objects/" ..binary_directory.. "/%{prj.name}" )

  files {
    "source/**.hpp",
    "source/**.cpp",
    "source/**.h",
    "source/**.c"
  }

  includedirs {
    "source",
    "%{include_directories.spdlog}",
    "%{include_directories.glfw}",
    "%{include_directories.glm}"
  }
