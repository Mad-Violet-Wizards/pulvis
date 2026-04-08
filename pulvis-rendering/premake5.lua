project "pulvis-rendering"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    
    files { 
        "%{wks.location}/pulvis-rendering/src/**.hpp", 
        "%{wks.location}/pulvis-rendering/src/**.cpp", 
        "%{wks.location}/pulvis-rendering/src/**.c"
    }

    includedirs { "%{wks.location}/pulvis-template-library/src",
                  "%{wks.location}/pulvis-core/src",
                  "%{wks.location}/pulvis-events/src",
                  "%{wks.location}/pulvis-filesystem/src",
                  "%{wks.location}/pulvis-rendering/src",
                  "%{wks.location}/pulvis-vendor/common/include/" }

    filter "system:windows"
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }
        libdirs { "%{wks.location}/pulvis-vendor/windows/bin/" }
        defines { "GLFW_DLL" }
        links { "pulvis-core","pulvis-events","pulvis-filesystem","glfw3dll"}
        postbuildcommands {
            "{COPY} %{wks.location}/pulvis-vendor/windows/bin/glfw3.dll %{cfg.targetdir}"
        }

    filter "system:macosx"
        includedirs { "/opt/homebrew/include" }
        libdirs { "/opt/homebrew/lib" }
        links { "pulvis-core","pulvis-events","pulvis-filesystem","glfw" }
        links { "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }

    filter {}
    dependson { "pulvis-core", "pulvis-events","pulvis-filesystem", "pulvis-rendering", "pulvis-level" }

project "pulvis-rendering-playground"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    files { 
        "%{wks.location}/pulvis-rendering/playground/include/**.hpp", 
        "%{wks.location}/pulvis-rendering/playground/src/**.cpp" 
    }

    includedirs { 
        "%{wks.location}/pulvis-template-library/src",
        "%{wks.location}/pulvis-core/src",
        "%{wks.location}/pulvis-filesystem/src",
        "%{wks.location}/pulvis-rendering/src",
        "%{wks.location}/pulvis-vendor/common/include/",
    }

    filter "system:windows"
        includedirs { "%{wks.location}/pulvis-vendor/windows/include/" }
        libdirs { "%{wks.location}/pulvis-vendor/windows/bin/" }
        links { "glfw3dll", "pulvis-rendering", "pulvis-filesystem", "pulvis-core" }

    filter "system:macosx"
        includedirs { "/opt/homebrew/include" }
        libdirs { "/opt/homebrew/lib" }
        links { "glfw", "pulvis-rendering", "pulvis-filesystem", "pulvis-core" }
        links { "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }

    filter {}
    dependson { "glfw3dll", "pulvis-rendering", "pulvis-filesystem", "pulvis-core" }
