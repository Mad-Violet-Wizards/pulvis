local RootDir = "../../../.."

project "Engine"
    kind "StaticLib"
    basedir(RootDir)
    language "C++"
    cppdialect "C++20"
    targetdir(RootDir .. "build/%{cfg.buildcfg}")

    files { RootDir .. "/engine/**.hpp", RootDir .. "/engine/**.cpp" }

    filter "configurations:*"
        includedirs{ "$(VULKAN_SDK)/macOS/Include/", RootDir .. "/vendor/macos/include/", RootDir .. "/vendor/common/include/", RootDir }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"

project "Game"
    kind "ConsoleApp"
    basedir(RootDir)
    language "C++"
    cppdialect "C++20"
    targetdir(RootDir .. "build/%{cfg.buildcfg}")

    files { RootDir .. "/game/**.hpp", RootDir .. "/game/**.cpp" }

    filter "configurations:*"
        includedirs{ "$(VULKAN_SDK)/macOS/Include/", RootDir .. "/vendor/macos/include/", RootDir .. "/vendor/common/include/", RootDir }
        libdirs{ "$(VULKAN_SDK)/macOS/lib/", RootDir .. "/vendor/macos/lib-arm/", RootDir .. "/vendor/common/lib/"}
        links { "Engine", "vulkan", "MoltenVK" ,"glfw3", "OpenGL.framework", "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }
        linkoptions { "-Wl,-rpath,$(VULKAN_SDK)/macOS/lib/" }
    
        filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"      
