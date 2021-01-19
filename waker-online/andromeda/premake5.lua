project "andromeda"
  kind "StaticLib"
  language "C++"
  staticruntime "on"
  cppdialect "C++17"

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
    "%{include_directories.glfw}"
  }

  links {
    "glfw3"
  }
