require "pulvis-scripts.build-tools.common.cache"
require "pulvis-scripts.build-tools.common.commands"
require "pulvis-scripts.build-tools.common.filepath"

project "pulvis-events"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        PULVIS_ROOT .. "/pulvis-events/src/**.hpp",
        PULVIS_ROOT .. "/pulvis-events/src/**.cpp"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-rtti/src",
        PULVIS_ROOT .. "/pulvis-core/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/",
        PULVIS_ROOT .. "/pulvis-events/src"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

    filter {}
    links { "pulvis-template-library", "pulvis-rtti", "pulvis-core" }
    dependson { "pulvis-template-library", "pulvis-rtti", "pulvis-core" }

project "pulvis-events-tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++latest"
    targetdir "%{wks.location}/build/%{cfg.buildcfg}"

    files {
        PULVIS_ROOT .. "/pulvis-events/tests/**.hpp",
        PULVIS_ROOT .. "/pulvis-events/tests/**.cpp"
    }

    includedirs {
        PULVIS_ROOT .. "/pulvis-template-library/src",
        PULVIS_ROOT .. "/pulvis-rtti/src",
        PULVIS_ROOT .. "/pulvis-core/src",
        PULVIS_ROOT .. "/pulvis-vendor/common/include/",
        PULVIS_ROOT .. "/pulvis-events/src",
        PULVIS_ROOT .. "/pulvis-events/tests"
    }

    filter "system:windows"
        buildoptions { "/utf-8" }
        includedirs { PULVIS_ROOT .. "/pulvis-vendor/windows/include/" }

    filter {}

    links { "pulvis-template-library", "pulvis-rtti", "pulvis-core", "pulvis-events" }
    dependson { "pulvis-template-library", "pulvis-rtti", "pulvis-core", "pulvis-events" }