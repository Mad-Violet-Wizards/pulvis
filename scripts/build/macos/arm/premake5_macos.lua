project "Engine"
    kind "SharedLib"
    language "C++"
    cppdialect "C++20"
    targetdir("%{wks.location}/build/%{cfg.buildcfg}")
    objdir("%{wks.location}/build/%{cfg.buildcfg}/obj/")
    basedir("../../../../")

    defines { "PULVIS_EXPORTS", "FMT_HEADER_ONLY", "MAC_OS" }
    pchheader("engine/engine_pch.hpp")
    pchsource("%{wks.location}/engine/engine_pch.cpp")
    files { "%{wks.location}/engine/**.hpp", "%{wks.location}/engine/**.cpp" }

    filter "configurations:*"
        includedirs{ 
            "$(VULKAN_SDK)/macOS/Include/", 
            "%{wks.location}/vendor/macos/include/", 
            "%{wks.location}/vendor/common/include/", 
            "%{wks.location}"}

        libdirs {
            "$(VULKAN_SDK)/macOS/lib/",
            "%{wks.location}/vendor/macos/lib-arm/"
        }

        buildoptions { "-stdlib=libc++", "-fPIC" }
        links { "vulkan", "MoltenVK", "glfw3", "OpenGL.framework", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }
        linkoptions { "-Wl,-rpath,$(VULKAN_SDK)/macOS/lib/", "-stdlib=libc++", "-lc++abi", "-v"}

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
    basedir("../../../../")

    files { "%{wks.location}/game/**.hpp", "%{wks.location}/game/**.cpp" }
    
    filter "configurations:*"
        includedirs { 
            "$(VULKAN_SDK)/macOS/Include/", 
            "%{wks.location}/vendor/macos/include/", 
            "%{wks.location}/vendor/common/include/", 
            "%{wks.location}" 
        }

        libdirs {
            "$(VULKAN_SDK)/macOS/lib/",
            "%{wks.location}/vendor/macos/lib-arm/",
            "%{cfg.targetdir}"
        }

        buildoptions { "-stdlib=libc++", "-fPIC" }
        links { "Engine", "vulkan", "MoltenVK", "glfw3", "OpenGL.framework", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }
        linkoptions { "-Wl,-rpath,$(VULKAN_SDK)/macOS/lib/", "-stdlib=libc++", "-lc++abi", "-v" }
    
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
    basedir("../../../../")

    files { "%{wks.location}/playground/**.hpp", "%{wks.location}/playground/**.cpp" }

    filter "configurations:*"
        includedirs {
            "$(VULKAN_SDK)/macOS/Include/", 
            "%{wks.location}/vendor/macos/include/", 
            "%{wks.location}/vendor/common/include/",
            "%{wks.location}"
        }

        libdirs {
            "$(VULKAN_SDK)/macOS/lib/",
            "%{wks.location}/vendor/macos/lib-arm/",
            "%{cfg.targetdir}"
        }

        buildoptions { "-stdlib=libc++", "-fPIC"}
        links { "Engine", "vulkan", "MoltenVK", "glfw3", "OpenGL.framework", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }
        linkoptions { "-Wl,-rpath,$(VULKAN_SDK)/macOS/lib/", "-stdlib=libc++", "-lc++abi", "-v" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"  

