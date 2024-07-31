
project "Engine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    targetdir("%{wks.location}/build/%{cfg.buildcfg}")

    defines { "GLFW_DLL", "PULVIS_EXPORTS" }
    pchheader("engine/engine_pch.hpp")
    pchsource("%{wks.location}/engine/engine_pch.cpp")
    files { "%{wks.location}/engine/**.hpp", "%{wks.location}/engine/**.cpp" }


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

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"

project "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir("%{wks.location}/build/%{cfg.buildcfg}")

    files { "%{wks.location}/game/**.hpp", "%{wks.location}/game/**.cpp" }

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

        links { "Engine", "glfw3dll" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"      

project "Playground"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    targetdir("%{wks.location}/build/%{cfg.buildcfg}")

    files { "%{wks.location}/playground/**.hpp", "%{wks.location}/playground/**.cpp" }

    filter "configurations:*"
        includedirs {
            "%{wks.location}",
            "%{wks.location}/vendor/common/include/"
        }

        libdirs {
            "%{cfg.targetdir}"
        }

        links { "Engine" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"  
