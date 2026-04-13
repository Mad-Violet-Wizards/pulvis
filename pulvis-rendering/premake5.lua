project "pulvis-rendering"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    
    files { 
        PULVIS_ROOT .. "/pulvis-rendering/src/**.hpp", 
        PULVIS_ROOT .. "/pulvis-rendering/src/**.cpp", 
        PULVIS_ROOT .. "/pulvis-rendering/src/**.c"
    }

    includedirs { PULVIS_ROOT .. "/pulvis-template-library/src",
                  PULVIS_ROOT .. "/pulvis-rtti/src",
                  PULVIS_ROOT .. "/pulvis-core/src",
                  PULVIS_ROOT .. "/pulvis-events/src",
                  PULVIS_ROOT .. "/pulvis-filesystem/src",
                  PULVIS_ROOT .. "/pulvis-rendering/src",
                  PULVIS_ROOT .. "/pulvis-vendor/common/include/" }

    filter "system:windows"
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }
        libdirs { PULVIS_ROOT .. "/pulvis-vendor/windows/bin/" }
        defines { "GLFW_DLL" }
        links { "pulvis-core","pulvis-events","pulvis-filesystem","pulvis-rtti","glfw3dll"}
        postbuildcommands {
            "{COPY} " .. PULVIS_ROOT .. "/pulvis-vendor/windows/bin/glfw3.dll %{cfg.targetdir}"
        }

    filter "system:macosx"
        includedirs { "/opt/homebrew/include" }
        libdirs { "/opt/homebrew/lib" }
        links { "pulvis-core","pulvis-events","pulvis-filesystem","pulvis-rtti","glfw" }
        links { "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }

    filter {}
    dependson { "pulvis-core", "pulvis-rtti", "pulvis-events", "pulvis-filesystem" }

project "pulvis-rendering-playground"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    files { 
        PULVIS_ROOT .. "/pulvis-rendering/playground/include/**.hpp", 
        PULVIS_ROOT .. "/pulvis-rendering/playground/src/**.cpp" 
    }

    includedirs { 
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-rtti/src",
        PULVIS_ROOT .. "/pulvis-core/src",
        PULVIS_ROOT .. "/pulvis-filesystem/src",
        PULVIS_ROOT .. "/pulvis-rendering/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/",
    }

    filter "system:windows"
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }
        libdirs { PULVIS_ROOT .. "/pulvis-vendor/windows/bin/" }
        links { "glfw3dll", "pulvis-template-library", "pulvis-rendering", "pulvis-filesystem", "pulvis-core", "pulvis-rtti" }

    filter "system:macosx"
        includedirs { "/opt/homebrew/include" }
        libdirs { "/opt/homebrew/lib" }
        links { "glfw", "pulvis-template-library", "pulvis-rendering", "pulvis-filesystem", "pulvis-core", "pulvis-rtti" }
        links { "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }

    filter {}
    dependson { "glfw3dll", "pulvis-template-library", "pulvis-rendering", "pulvis-filesystem", "pulvis-core", "pulvis-rtti" }
