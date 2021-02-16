project "waker-online"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"

  targetdir ( "binary/" ..binary_directory.. "/%{prj.name}" )
  objdir ( "binary/objects/" ..binary_directory.. "/%{prj.name}" )

  links {
    "andromeda"
  }

  files {
    "source/**.hpp",
    "source/**.cpp",
    "source/**.h",
    "source/**.c",
  }

  filter "files:**.vert" 
      buildmessage "Compiling shaders"
      buildcommands {
        "glslc -o %{file.directory}/binary/%{file.name}.spv %{file.relpath}"
      }

      buildoutputs {
        "%{cfg.objdir}/%{file.directory}/%{file.name}.spv"
      }
  filter {}

  includedirs {
    "source",
    "%{wks.location}/waker-online/andromeda/source",
    "%{include_directories.spdlog}",
    "%{include_directories.glfw}"
  }
