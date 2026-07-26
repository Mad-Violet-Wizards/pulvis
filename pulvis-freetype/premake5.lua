require "pulvis-scripts.build-tools.common.cache"
require "pulvis-scripts.build-tools.common.commands"
require "pulvis-scripts.build-tools.common.filepath"

project "pulvis-freetype"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"
    warnings "Off"

    defines { "FT2_BUILD_LIBRARY", "_CRT_SECURE_NO_WARNINGS" }

    files {
        PULVIS_ROOT .. "/pulvis-freetype/src/**.h",
        PULVIS_ROOT .. "/pulvis-freetype/src/**.c"
    }

    removefiles { PULVIS_ROOT .. "/pulvis-freetype/src/gzip/*.c" }
    files { PULVIS_ROOT .. "/pulvis-freetype/src/gzip/ftgzip.c" }

    includedirs {
        PULVIS_ROOT .. "/pulvis-freetype/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

    filter {}
