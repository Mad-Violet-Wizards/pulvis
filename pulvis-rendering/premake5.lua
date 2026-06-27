-- The pulvis-rendering library has been merged into the consolidated
-- pulvis-engine target. Only the standalone playground app remains here.
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
        links { "glfw3dll", "pulvis-template-library", "pulvis-rtti", "pulvis-engine" }

    filter "system:macosx"
        includedirs { "/opt/homebrew/include" }
        libdirs { "/opt/homebrew/lib" }
        links { "glfw", "pulvis-template-library", "pulvis-rtti", "pulvis-engine" }
        links { "Cocoa.framework", "IOKit.framework", "CoreVideo.framework" }

    filter {}
    dependson { "glfw3dll", "pulvis-template-library", "pulvis-rtti", "pulvis-engine" }
