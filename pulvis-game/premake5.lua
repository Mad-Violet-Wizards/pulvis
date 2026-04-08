project "pulvis-game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        "%{wks.location}/pulvis-game/src/**.hpp",
        "%{wks.location}/pulvis-game/src/**.cpp"
    }

    includedirs {
        "%{wks.location}/pulvis-game/src",
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-rtti/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-events/src",
        "%{wks.location}/pulvis-threads/src",
        "%{wks.location}/pulvis-filesystem/src",
        "%{wks.location}/pulvis-rendering/src",
        "%{wks.location}/pulvis-level/src",
        "%{wks.location}/pulvis-game-engine/src",

        "%{wks.location}/pulvis-vendor/common/include/",
    }

    filter "system:windows"
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }
        libdirs { "%{wks.location}/pulvis-vendor/windows/bin/" }
        defines { "GLFW_DLL" }
        links { "pulvis-template-library", "pulvis-rtti", "pulvis-core", "pulvis-threads", "pulvis-filesystem", "pulvis-rendering", "pulvis-level", "pulvis-game-engine", "pulvis-events", "glfw3dll" }
        postbuildcommands {
            "{COPY} %{wks.location}/pulvis-vendor/windows/bin/glfw3.dll %{cfg.targetdir}"
        }

    filter "system:macosx"
        includedirs { "/opt/homebrew/include" }
        libdirs { "/opt/homebrew/lib" }
        links { "pulvis-template-library", "pulvis-rtti", "pulvis-core", "pulvis-threads", "pulvis-filesystem", "pulvis-rendering", "pulvis-level", "pulvis-game-engine", "pulvis-events", "glfw" }
        links { "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }

    filter {}
    dependson { "pulvis-template-library", "pulvis-rtti", "pulvis-core", "pulvis-threads", "pulvis-filesystem", "pulvis-rendering", "pulvis-level", "pulvis-game-engine", "pulvis-events" }
