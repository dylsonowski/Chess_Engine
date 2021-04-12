workspace "Chess_Engine"
    architecture "x86_64"
    configurations {
        "Debug",
        "Release"
    }

project "Pale_Engine"
    location "Pale_Engine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/%{prj.name}_%{cfg.architecture}/%{cfg.buildcfg}")
    objdir ("bin-int/%{prj.name}_%{cfg.architecture}/%{cfg.buildcfg}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "PLATFORM_WINDOWS",
            "BUILD_DLL"
        }

        postbuildcommands {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/Chess_Game_%{cfg.architecture}/%{cfg.buildcfg}")
        }

    filter "configurations:Debug"
        defines "_DEBUG_"
        symbols "On"

    filter "configurations:Release"
        defines "_RELEASE_"
        optimize "On"


project "Chess_Game"
    location "Chess_Game"
    kind "ConsoleApp"
    language "C++"
    
    targetdir ("bin/%{prj.name}_%{cfg.architecture}/%{cfg.buildcfg}")
    objdir ("bin-int/%{prj.name}_%{cfg.architecture}/%{cfg.buildcfg}")
    
    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    includedirs {
        "Pale_Engine/vendor/spdlog/include",
        "Pale_Engine/src"
    }

    links {
        "Pale_Engine"
    }
    
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "_DEBUG_"
        symbols "On"

    filter "configurations:Release"
        defines "_RELEASE_"
        optimize "On"