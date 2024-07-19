local RootDir = "../../.."
local VulkanSdkPath = os.getenv("VULKAN_SDK")
print("VulkanSDK path: ", VulkanSdkPath)

project "Engine"
    kind "StaticLib"
    basedir(RootDir)
    language "C++"
    cppdialect "C++20"
    targetdir(RootDir .. "build/%{cfg.buildcfg}")

    files { RootDir .. "/engine/**.hpp", RootDir .. "/engine/**.cpp" }

    filter "configurations:*"
        includedirs { VulkanSdkPath .. "/Include/", RootDir .. "/vendor/windows/include/", RootDir }
        libdirs{ VulkanSdkPath .. "/Lib32/", RootDir .. "/vendor/windows/lib/" }
        links { "vulkan-1", "glfw3", "opengl32", "User32" }

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
        includedirs { VulkanSdkPath .. "/Include/", RootDir .. "/vendor/windows/include/", RootDir }
        libdirs{ VulkanSdkPath .. "/Lib32/", RootDir .. "/vendor/windows/lib/" }
        links { "Engine" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"      
