workspace "Chess_Engine"
    architecture "x86_64"
    configurations {
        "Debug",
        "Release"
    }

project "Pale_Engine"
    location "Pale_Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/%{prj.name}_%{cfg.architecture}/%{cfg.buildcfg}")
    objdir ("bin-int/%{prj.name}_%{cfg.architecture}/%{cfg.buildcfg}")

    pchheader "palepch.h"
    pchsource "Pale_Engine/src/palepch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src"
    }

    filter "system:windows"
        systemversion "latest"

        defines {
            "PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "_DEBUG_"
        symbols "on"

    filter "configurations:Release"
        defines "_RELEASE_"
        optimize "on"


project "Chess_Game"
    location "Chess_Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    
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
        systemversion "latest"

        defines {
            "PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "_DEBUG_"
        symbols "on"

    filter "configurations:Release"
        defines "_RELEASE_"
        optimize "on"


project "Automation_Tests"
    location "Automation_Tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

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

    defines "TESTING_ENV"

    filter "system:windows"
        systemversion "latest"

        defines {
            "PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "_DEBUG_"
        symbols "on"

    filter "configurations:Release"
        defines "_RELEASE_"
        optimize "on"