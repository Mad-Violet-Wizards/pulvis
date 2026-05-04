require "pulvis-scripts.build-tools.common.cache"
require "pulvis-scripts.build-tools.common.commands"
require "pulvis-scripts.build-tools.common.filepath"

project "pulvis-scriptable"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    
    files {
        PULVIS_ROOT .. "/pulvis-scriptable/src/**.hpp", 
        PULVIS_ROOT .. "/pulvis-scriptable/src/**.cpp", 
        PULVIS_ROOT .. "/pulvis-scriptable/src/**.c"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-rtti/src",
        PULVIS_ROOT .. "/pulvis-core/src",
        PULVIS_ROOT .. "/pulvis-scriptable/src",
        PULVIS_ROOT .. "/pulvis-filesystem/src",
        PULVIS_ROOT .. "/pulvis-events/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" } -- FMT library requires UTF-8 encoding
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-template-library", "pulvis-rtti", "pulvis-core", "pulvis-filesystem", "pulvis-events" }
    dependson { "pulvis-template-library", "pulvis-rtti", "pulvis-core", "pulvis-filesystem", "pulvis-events" }