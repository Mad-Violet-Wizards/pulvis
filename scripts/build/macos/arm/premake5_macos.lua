local RootDir = "../../../.."

project "Engine"
    kind "StaticLib"
    basedir(RootDir)
    language "C++"
    cppdialect "C++20"
    targetdir(RootDir .. "build/%{cfg.buildcfg}")

    files { RootDir .. "/engine/**.hpp", RootDir .. "/engine/**.cpp" }
    includedirs(RootDir)

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
    includedirs(RootDir)

    filter "configurations:*"
        -- includedirs { "vendor/include/", "." }
        -- libdirs { "vendor/lib/" }
        links { "Engine" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"      
