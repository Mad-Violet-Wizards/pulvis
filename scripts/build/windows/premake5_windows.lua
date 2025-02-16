
project "Engine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++latest"
    targetdir("%{wks.location}/build/%{cfg.buildcfg}")
    basedir("../../../")

    defines { "GLFW_DLL", "PULVIS_EXPORTS", "WINDOWS_OS", "FMT_HEADER_ONLY" }
    pchheader("engine/engine_pch.hpp")
    pchsource("%{wks.location}/engine/engine_pch.cpp")
    files { "%{wks.location}/engine/**.hpp", "%{wks.location}/engine/**.cpp", "%{wks.location}/engine/**.h", "%{wks.location}/engine/**.c" }
    buildoptions{ "/utf-8", "/Zc:preprocessor"}
    flags("FatalWarnings")

    filter { "files:**/vendor/glad.c" }
        flags { "NoPCH" }

    filter "configurations:*"
        includedirs { 
            "$(VULKAN_SDK)/Include/", 
            "%{wks.location}/vendor/common/include/", 
            "%{wks.location}/vendor/windows/include/", 
            "%{wks.location}"
        }

        libdirs { 
            "$(VULKAN_SDK)/Lib32/", 
            "%{wks.location}/vendor/windows/lib/" 
        }

        links { "vulkan-1", "glfw3dll" }
        postbuildcommands {
            "{COPYFILE} %{wks.location}/vendor/windows/bin/glfw3.dll %{cfg.targetdir}"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        buildoptions { "/ZI", "/MP" }
        linkoptions { "/INCREMENTAL" }

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"

project "Game"
    defines { "FMT_HEADER_ONLY" }
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir("%{wks.location}/build/%{cfg.buildcfg}")
    basedir("../../../")
    buildoptions{ "/utf-8", "/Zc:preprocessor" }

    files { "%{wks.location}/game/**.hpp", "%{wks.location}/game/**.cpp", "%{wks.location}/game/**.h", "%{wks.location}/game/**.c" }

    filter "configurations:*"
        includedirs { 
            "$(VULKAN_SDK)/Include/", 
            "%{wks.location}/vendor/windows/include/", 
            "%{wks.location}/vendor/common/include/",
            "%{wks.location}" 
        }

        libdirs { 
            "$(VULKAN_SDK)/Lib32/", 
            "%{wks.location}/vendor/windows/lib/",
            "%{cfg.targetdir}"
        }

        links { "Engine", "vulkan-1", "glfw3dll" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        buildoptions { "/ZI", "/MP" }
        linkoptions { "/INCREMENTAL" }

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"      

project "Playground"
    defines { "FMT_HEADER_ONLY" }
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir("%{wks.location}/build/%{cfg.buildcfg}")
    basedir("../../../")
    buildoptions{ "/utf-8", "/Zc:preprocessor" }

    files { "%{wks.location}/playground/**.hpp", "%{wks.location}/playground/**.cpp", "%{wks.location}/playground/**.h", "%{wks.location}/playground/**.c" }

    filter "configurations:*"
        includedirs {
            "$(VULKAN_SDK)/Include/", 
            "%{wks.location}/vendor/windows/include/", 
            "%{wks.location}/vendor/common/include/",
            "%{wks.location}" 
        }

        libdirs {
            "$(VULKAN_SDK)/Lib32/", 
            "%{wks.location}/vendor/windows/lib/",
            "%{cfg.targetdir}"        
        }

        links { "Engine", "vulkan-1", "glfw3dll" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"  
