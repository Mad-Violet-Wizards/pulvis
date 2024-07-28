local RootDir = "../../.."

project "Engine"
    kind "StaticLib"
    basedir(RootDir)
    language "C++"
    cppdialect "C++20"
    targetdir(RootDir .. "build/%{cfg.buildcfg}")

    files { RootDir .. "/engine/**.hpp", RootDir .. "/engine/**.cpp" }

    filter "configurations:*"
        includedirs { "$(VULKAN_SDK)/Include/", RootDir .. "/vendor/common/include/", RootDir .. "/vendor/windows/include/", RootDir }
        libdirs{ "$(VULKAN_SDK)/Lib32/", RootDir .. "/vendor/common/lib/", RootDir .. "/vendor/windows/lib/" }
        links { "vulkan-1", "glfw3" }

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
        includedirs { "$(VULKAN_SDK)/Include/", RootDir .. "/vendor/windows/include/", RootDir }
        libdirs{  "$(VULKAN_SDK)/Lib32/", RootDir .. "/vendor/windows/lib/" }
        links { "Engine" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"      
